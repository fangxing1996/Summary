#ifndef _MUTEX_H
#define _MUTEX_H
#include <pthread.h>
class Mutex {

    public:
        Mutex();
        ~Mutex();
        int Mutex_lock();
        int Mutex_unlock();
        class AutoLock {//嵌套类
            
            public:
                AutoLock(Mutex& mtxp);
                ~AutoLock();

            private:
                Mutex& refmtx;

        };
    private:
        pthread_mutex_t* mtxPtr;  
};
#endif