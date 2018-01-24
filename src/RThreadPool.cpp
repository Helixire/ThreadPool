#include "RThreadPool.h"
#include "RThread.h"

RPTR::ThreadPool::ThreadPool(const unsigned int nb)
    : m_data(new in_data), m_nb(nb) {
  m_data->running = true;
  m_data->run = 0;
  init(nb);
}

RPTR::ThreadPool::~ThreadPool() {
  m_data->running = false;
  m_data->sem.add(m_nb);
  for (int i = 0; i < m_nb; ++i) {
    m_pool[i].join();
  }
  delete[] m_pool;
}

void RPTR::ThreadPool::add_task(SCommand cmd) {
  {
    Locker lock(m_data->mut);
    m_data->list.push(cmd);
  }
  m_data->sem.post();
}

void RPTR::ThreadPool::wait() {
  while (1) {
    {
      Locker lock(m_data->mut);
      if (!m_data->run.get() && m_data->list.empty()) {
        break;
      }
    }
    m_data->update.wait();
  }
}

void RPTR::ThreadPool::init(unsigned int nb) {
  if (!nb)
    nb = 4; // TODO auto find correct number of thread
  m_nb = nb;
  m_pool = new Thread[nb];
  for (; nb; --nb) {
    m_pool[nb - 1].start((void (*)(void *))thread_main, &m_data);
  }
}

void RPTR::ThreadPool::thread_main(std::shared_ptr<in_data> *indata) {
  std::shared_ptr<in_data> data(*indata);
  SCommand cmd;

  while (data->running.get()) {
    data->sem.wait();
    if (!data->running.get())
      break;
    {
      Locker lock(data->mut);
      data->run += 1;
      cmd = data->list.front();
      data->list.pop();
    }
    cmd->execute();
    data->run -= 1;
    data->update.post();
  }
}
