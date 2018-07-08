#include "Condition.h"
#include "MutexLock.h"
#include <assert.h>

Condition::Condition(MutexLock &mutex):mutex_(mutex)
{
    CHECK(!pthread_cond_init(&cond_, NULL));//条件变量初始化
}

Condition::~Condition()
{
    CHECK(!pthread_cond_destroy(&cond_));//析构操作
}


void Condition::wait()
{
    assert(mutex_.isLocking()); //wait前必须上锁
    CHECK(!pthread_cond_wait(&cond_, mutex_.getMutexPtr()));
    //pthread_cond_wait阻塞时释放锁，返回时会自动加锁
    mutex_.restoreMutexStatus(); //还原状态
}

void Condition::notify()
{
    CHECK(!pthread_cond_signal(&cond_));//通知等待线程队列中的线程
}

void Condition::notifyAll()
{
    CHECK(!pthread_cond_broadcast(&cond_));//通知所有等待线程
}