#ifndef THREAD_H
#define THREAD_H
#include <iostream>
#include <pthread.h>

using namespace std;

class Thread
{
private:
    //��ǰ�̵߳��߳�ID
    pthread_t tid;
    //�̵߳�״̬
    int threadStatus;
    //��ȡִ�з�����ָ��
    static void * thread_proxy_func(void * args);
    //�ڲ�ִ�з���
    void* run1();
public:
    //�̵߳�״̬���½�
    static const int THREAD_STATUS_NEW = 0;
    //�̵߳�״̬����������
    static const int THREAD_STATUS_RUNNING = 1;
    //�̵߳�״̬�����н���
    static const int THREAD_STATUS_EXIT = -1;
    //���캯��
    Thread();
    //�̵߳�����ʵ��
    virtual void run()=0;
    //��ʼִ���߳�
    bool start();
    //��ȡ�߳�ID
    pthread_t getThreadID();
    //��ȡ�߳�״̬
    int getState();
    //�ȴ��߳�ֱ���˳�
    void join();
    //�ȴ��߳��˳����߳�ʱ
    void join(unsigned long millisTime);
};

class MultiThread : public Thread
{
public:
    void run()
    {
        int number = 0;
        for (int i = 0; i < 10; i++)
        {
            cout << "Current number is " << number++;
            cout << " PID is " << getpid() << " TID is " << getThreadID() << endl;
            sleep(1);
        }
    }
};

#endif