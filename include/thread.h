/**
 * @file Thread.h
 * @comment
 *
 * @author niexw
 */

#ifndef UNP_THREAD_H
#define UNP_THREAD_H


#include <pthread.h>
#include <list>
#include <boost/typeof/typeof.hpp>
#include <boost/utility.hpp>
#include "Exception.h"

#include <boost/ref.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>

int logLevel=0;

#define CALLBACK_PARAMS(w) &w.run, &w

namespace xcom
{

    typedef void* (*CallbackFunc)(void *);

/**
 * @class Thread
 * @comment
 *
 */
    class Thread
    {
    public:
        // thread 的属性
        enum Attribute { StackSize = 0, Affinity, Priority };

    protected:
        pthread_t tid_;
        pthread_attr_t attr_;

        void exit()
        { pthread_exit(NULL); }
        void exit(void *value)
        { pthread_exit(value); }

    public:
        Thread()
        { pthread_attr_init(&attr_); }
        ~Thread()
        {
            if (isRunning()) throw EXCEPTION();
            pthread_attr_destroy(&attr_);
        }

        // @TODO
        void setAttribute(Attribute &a, void *value)
        {
            switch(a)
            {
                case StackSize:
                    pthread_attr_setstacksize(&attr_, (size_t)value);
                    break;
                case Priority:
                    pthread_attr_setschedparam(&attr_, (const struct sched_param *)value);
                    break;
                default:
                    break;
            }
        }
        void getAttribute(Attribute &a, void *value)
        {
            switch(a)
            {
                case StackSize:
                    pthread_attr_getstacksize(&attr_, (size_t *)value);
                    break;
                case Priority:
                    pthread_attr_getschedparam(&attr_, (struct sched_param *)value);
                    break;
                default:
                    break;
            }
        }

        bool isRunning()
        { return pthread_kill(tid_, 0) == 0 ? true : false; }

        void detach()
        { if (pthread_detach(tid_) != 0) throw EXCEPTION(); }
        void detach(pthread_t tid)
        { if (pthread_detach(tid) != 0) throw EXCEPTION(); }

        void cancel()
        {
            if (!isRunning()) {
                tid_ = 0;
                return;
            }
            pthread_cancel(tid_);
            join();		// force the thread to be canceled.
        }
        void kill(int sig)
        { if (pthread_kill(tid_, sig) != 0) throw EXCEPTION(); }

        void join()
        { pthread_join(tid_, NULL); }

        pthread_t run(CallbackFunc f, void *p)
        {
            pthread_create(&tid_, &attr_, f, p);
        }
    };

    template <typename T, bool>
    struct CallbackWrapperImp
    {
        static void* run(void *p)
        {
            T *self = (T*)p;
            return (void*)self->func_();
        }
    };

    template <typename T>
    struct CallbackWrapperImp<T, true>
    {
        static void* run(void *p)
        {
            T *self = (T*)p;
            self->func_();
            return NULL;
        }
    };

    template <typename R>
    struct CallbackWrapper
            : CallbackWrapperImp<CallbackWrapper<R>, boost::is_void<R>::value>
    {
        boost::function<R ()> func_;

        CallbackWrapper()
        { }
        CallbackWrapper(boost::function<R ()> f) : func_(f)
        { }

        template <typename T>
        void set(T t)
        { func_ = t; }

        bool enabled()
        { return func_; }
    };

    struct PThreadMutex
    {
        pthread_mutex_t mutex_;

        PThreadMutex()
        { pthread_mutex_init(&mutex_, NULL); }
        ~PThreadMutex()
        { pthread_mutex_destroy(&mutex_); }

        void lock()
        { if (pthread_mutex_lock(&mutex_) != 0) throw EXCEPTION(); }

        void unlock()
        { if (pthread_mutex_unlock(&mutex_) != 0) throw EXCEPTION(); }

        bool trylock()
        {
            if(pthread_mutex_trylock(&mutex_))
                return false;
            else
                return true;
        }
    };

    struct PThreadCond
    {
        pthread_cond_t cond_;
        timespec time_;

        PThreadCond()
        {
            time_.tv_sec = -1;
            time_.tv_nsec = -1;
            pthread_cond_init(&cond_, NULL);
        }
        ~PThreadCond()
        { pthread_cond_destroy(&cond_); }

        void setTimeout(long sec, long nsec)
        {
            time_.tv_sec = sec;
            time_.tv_nsec = nsec;
        }

        void wait(PThreadMutex &m)
        { if (pthread_cond_wait(&cond_, &m.mutex_) != 0) throw EXCEPTION(); }

        void signal()
        { if (pthread_cond_signal(&cond_) != 0) throw EXCEPTION(); }

        bool timedWait(PThreadMutex &m)
        {
            if (pthread_cond_timedwait(&cond_, &m.mutex_, &time_) != 0) throw EXCEPTION();
        }
    };

    class ThreadPool : private boost::noncopyable
    {
    public:
        struct Task
        {
            CallbackFunc func;
            void *param;
            PThreadCond &cond;
            PThreadMutex &mutex;
            int &status;

            Task(CallbackFunc f, void *p,
                 PThreadCond &c, PThreadMutex &m, int &s)
                    : func(f), param(p), cond(c), mutex(m), status(s)
            { }

            void wait()
            {
                mutex.lock();
                while(status <= 0)
                    cond.wait(mutex);
                status--;
                mutex.unlock();
            }
        };

    protected:
        enum { IDLE = 0, BUSY };
        class Item : public Thread
        {
        protected:
            std::list<Task*> &handlers_;
            PThreadMutex &mutex_;
            PThreadCond &cond_;
            Task *handler_;

            // pthread_cond_wait will lock the mutex, so release it
            static void cleanup_(void *p)
            { ((Item*)p)->mutex_.unlock(); }
            static void* run_(void *self)
            {
                while(1)
                {
                    Item *pthis = (Item *)self;
                    pthis->mutex_.lock();
                    while(pthis->handlers_.size()==0)
                        pthis->cond_.wait(pthis->mutex_);
                    pthis->handler_ = pthis->handlers_.back();
                    pthis->handlers_.pop_back();
                    pthis->mutex_.unlock();
                    pthis->handler_->param = pthis->handler_->func(pthis->handler_->param);
                    pthis->handler_->mutex.lock();
                    pthis->handler_->status++;
                    pthis->handler_->cond.signal();
                    pthis->handler_->mutex.unlock();
                }
            }

        public:
            Item(std::list<Task*> &h, PThreadMutex &m, PThreadCond &c)
                    : handlers_(h), mutex_(m), cond_(c), handler_(NULL), status_(IDLE)
            { }

            ~Item()
            { cancel(); }

            int status_;	// 0 idle; 1 busy

            pthread_t run()
            {
                if (pthread_create(&tid_, &attr_, &Item::run_, this) == 0)
                    return tid_;
                else
                    throw EXCEPTION();	// EAGAIN means no memory
            }
        };

        PThreadMutex mutex_;
        PThreadCond cond_;
        std::list<Task*> handlers_;
        std::list<Item*> threads_;

    public:
        explicit ThreadPool(size_t size)
        { add(size); }
        ~ThreadPool()
        { close(); }

        void add(size_t size)
        {
            for(int i = 0; i < size; i++)
            {
                Item *p = new Item(handlers_, mutex_, cond_);
                threads_.push_back(p);
                p->run();
            }
        }

        size_t remove(size_t size)
        {
            for(int i = 0; i < size; i++)
            {
                delete threads_.back();
                threads_.pop_back();
            }
        }

        void close()
        {
            while (!threads_.empty())
            {
                delete threads_.front();
                threads_.pop_front();
            }
        }

        size_t size()
        { return threads_.size(); }

        void addTask(Task &handler)
        {
            mutex_.lock();
            if(handlers_.size()==0)
                cond_.signal();
            handlers_.push_back(&handler);
            mutex_.unlock();
        }
    };

}; // namespace xcom

#endif //UNP_THREAD_H
