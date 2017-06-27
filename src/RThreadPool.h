#ifndef __RTHREADPOOL_H__
#define __RTHREADPOOL_H__

#include <memory>
#include <queue>
#include "RSemaphore.h"
#include "RLocker.h"
#include "RAtomic.h"

namespace RPTR
{
  class ThreadPool
  {
  public:
    //Number of thread in the thread pool
    ThreadPool(const unsigned int nb = 0);
    ~ThreadPool();

    void    add_task(void (*funct)(void *), void *data);
    void    wait();
  private:
    struct cmd
    {
      void    (*funct)(void*);
      void    *data;
    };
    struct in_data
    {
      Semaphore             sem;
      Mutex                 mut;
      std::queue<cmd>       list;
      Atomic<bool>          running;
      Semaphore             update;
      Atomic<unsigned int>  run;
    };

    void init(unsigned int nb);
    static void thread_main(std::shared_ptr<in_data> *ptr);

    std::shared_ptr<in_data>  m_data;
    unsigned int              m_nb;
  };
}

#endif /* __RTHREADPOOL_H__ */
