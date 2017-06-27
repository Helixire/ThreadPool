#include "RThreadPool.h"
#include "RThread.h"

RPTR::ThreadPool::ThreadPool(const unsigned int nb) : m_data(new in_data), m_nb(nb)
{
  m_data->running = true;
  m_data->run = 0;
  init(nb);
}

RPTR::ThreadPool::~ThreadPool()
{
  m_data->running = false;
  m_data->sem.add(m_nb);
}

void RPTR::ThreadPool::add_task(void (*funct)(void *), void *data)
{
  {
    Locker  lock(m_data->mut);
    m_data->list.push({funct, data});
  }
  m_data->sem.post();
}

void RPTR::ThreadPool::wait()
{
  while (1)
  {
    {
      Locker  lock(m_data->mut);
      if (!m_data->run.get() && m_data->list.empty())
      {
        break;
      }
    }
    m_data->update.wait();
  }
}

void RPTR::ThreadPool::init(unsigned int nb)
{
  Thread  *tmp;

  if (!nb)
  nb = 4; //TODO auto find correct number of thread
  m_nb = nb;
  for (; nb; --nb)
  {
    tmp = new Thread;
    tmp->start((void (*)(void *))thread_main, &m_data);
    tmp->detach();
    delete tmp;
  }
}


void RPTR::ThreadPool::thread_main(std::shared_ptr<in_data> *indata)
{
  std::shared_ptr<in_data> data(*indata);
  cmd tmp;

  while (data->running.get())
  {
    data->sem.wait();
    if (!data->running.get())
    break;
    {
      Locker  lock(data->mut);
      data->run += 1;
      tmp = data->list.front();
      data->list.pop();
    }
    tmp.funct(tmp.data);
    data->run -= 1;
    data->update.post();
  }
}
