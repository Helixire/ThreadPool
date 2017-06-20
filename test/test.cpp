#include <iostream>
#include "RThreadPool.h"
#include <unistd.h>

RPTR::Mutex  mut;

void    fun(int *nb)
{
  mut.lock();
  std::cout << (unsigned int)pthread_self() << " " << *nb << std::endl;
  mut.unlock();
  sleep(1);
  delete nb;
}

int main(int ac, char **av)
{
  int               i;
  int               *nb;
  RPTR::ThreadPool  pool(4);

    for (i = 0; i < 20; ++i)
    {
      nb = new int(i);
      pool.add_task((void (*)(void *))fun, (void *)nb);
    }
    pool.wait();
}
