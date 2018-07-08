#ifndef __CONDITION_H__
#define __CONDITION_H__
#include "MutexLock.h"
#include <pthread.h>
#include <boost/noncopyable.hpp>
#include <assert.h>
class MutexLock;
class Condition
{
    public:
        Condition(MutexLock &mutex);
        ~Condition();
        void wait();//封装pthread_cond_wait
        void notify();//封装pthread_cond_signal
        void notifyAll();//封装pthread_cond_broadcast
    private:
        pthread_cond_t cond_;
        MutexLock &mutex_;
};
#endif