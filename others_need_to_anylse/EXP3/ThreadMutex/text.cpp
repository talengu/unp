#include <pthread.h>
#include "tmutex.h"
#include <iostream>
using namespace std;

typedef ThreadMutex MUTEX_TYPE; //使用线程互斥量的互斥量类型
//typedef NullMutex MUTEX_TYPE; //不使用互斥量的互斥量类型

MUTEX_TYPE g_mtx; //互斥量变量定义
void *print_msg_thread(void *parg);

void *print_msg_thread(void *parg)
{//工作线程，用循环模拟一个的工作。
char *msg = (char *)parg;
AUTO_GUARD( gd, MUTEX_TYPE, g_mtx );
for(int i=0; i<10; i++ )
{
　　cout << msg << endl;
　　sleep( 1 );
}
return NULL;
}

int main()
{
pthread_t t1,t2;

//创建两个工作线程，第1个线程打印10个1，第2个线程打印10个2。
pthread_create( &t1, NULL, &print_msg_thread, (void *)"1" );
pthread_create( &t2, NULL, &print_msg_thread, (void *)"2" );

//等待线程结束
pthread_join( t1,NULL);
pthread_join( t2,NULL);
return 0;  
} 