#include "RThreadPool.h"

RPTR::ThreadPool::ThreadPool(const unsigned int nb) : m_nb(nb)
{
    m_data.running = true;
    m_data.run = 0;
    init(nb);
}

RPTR::ThreadPool::~ThreadPool()
{
    m_data.running = false;
    m_data.sem.add(m_nb);
    delete[] m_pool;
}

void RPTR::ThreadPool::add_task(void (*funct)(void *), void *data)
{
    m_data.mut.lock();
    m_data.list.push({funct, data});
    m_data.mut.unlock();
    m_data.sem.post();
}

void RPTR::ThreadPool::wait()
{
  while (1)
  {
    m_data.mut.lock();
    if (!m_data.run && m_data.list.empty())
      break;
    m_data.mut.unlock();
    m_data.update.wait();
  }
  m_data.mut.unlock();
}

void RPTR::ThreadPool::init(unsigned int nb)
{
    if (!nb)
        nb = 4; //TODO auto find correct number of thread
    m_nb = nb;
    m_pool = new Thread[nb];
    for (; nb; --nb)
    {
      m_pool[nb - 1].start((void (*)(void *))thread_main, &m_data);
      m_pool[nb - 1].detach();
    }
}


void RPTR::ThreadPool::thread_main(in_data *data)
{
    cmd tmp;

    while (data->running)
    {
        data->sem.wait();
        if (!data->running)
            break;
        data->mut.lock();
        ++data->run;
        tmp = data->list.front();
        data->list.pop();
        data->mut.unlock();
        tmp.funct(tmp.data);
        data->mut.lock();
        --data->run;
        data->mut.unlock();
        data->update.post();
    }
}
