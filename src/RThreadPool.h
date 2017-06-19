#ifndef __RTHREADPOOL_H__
#define __RTHREADPOOL_H__

#include <queue>
#include "RThread.h"
#include "RSemaphore.h"
#include "RMutex.h"

namespace RPTR
{
    class ThreadPool
    {
    public:
        //Number of thread in the thread pool
        ThreadPool(const unsigned int nb = 0);
        ~ThreadPool();

        void    add_task(void (*funct)(void *), void *data);
    private:
        struct cmd
        {
            void    (*funct)(void*);
            void    *data;
        };
        struct in_data
        {
            Semaphore       sem;
            Mutex           mut;
            std::queue<cmd> list;
            bool            running;
        };
        
        void init(unsigned int nb);
        static void thread_main(in_data *);
        
        in_data         m_data;
        Thread          *m_pool;
        unsigned int    m_nb;
    };
}

#endif /* __RTHREADPOOL_H__ */
