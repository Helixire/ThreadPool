#include "RThreadPool.h"

RPTR::ThreadPool::ThreadPool(const unsigned int nb) : m_nb(nb)
{
    m_data.running = true;
    init(nb);
}

RPTR::ThreadPool::~ThreadPool()
{
    m_data.running = false;
    m_data.sem.add(m_nb);
}

void RPTR::ThreadPool::add_task(void (*funct)(void *), void *data)
{
    m_data.mut.lock();
    m_data.list.push({funct, data});
    m_data.mut.unlock();
    m_data.sem.post();
}

void RPTR::ThreadPool::init(unsigned int nb)
{
    if (!nb)
        nb = 4; //TODO auto find correct number of thread
    m_nb = nb;
    m_pool = new Thread[nb];
    for (; nb; --nb)
        m_pool[nb].start((void (*)(void *))thread_main, &m_data);
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
        tmp = data->list.front();
        data->list.pop();
        data->mut.unlock();
        tmp.funct(tmp.data);
    }
}
