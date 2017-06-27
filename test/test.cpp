#include <iostream>
#include "RThreadPool.h"
#include <unistd.h>

RPTR::Mutex  mut;

void    fun(int *nb)
{
    {
    RPTR::Locker    lock(mut);
    std::cout << (unsigned int)pthread_self() << " " << *nb << std::endl;
    }
    delete nb;
}

void    instructor(RPTR::ThreadPool *pool)
{
    for (int i = 0; i < 10; ++i)
    {
        {
            RPTR::Locker    lock(mut);
            std::cout << "Pushed " << i << std::endl;
        }
        pool->add_task((void (*)(void*))fun, (void *)(new int(i)));
    }
}

int main(int ac, char **av)
{
    RPTR::ThreadPool  pool(4);

    pool.add_task((void (*)(void *))instructor, (void *)&pool);
    pool.wait();
}
