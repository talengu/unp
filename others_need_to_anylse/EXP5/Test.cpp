#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <unistd.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <stdint.h>
#include <pthread.h>
#include <iostream>
#include <map>
#include <algorithm>
#include <list>
#include "timer.h"


void timer_proc(void *args) {
    cout << args << endl;
}
int main() {
    list<Timer*> l;
    for(int i=0; i<10;++i) {
        Timer *t = new Timer();
        t->start(500, timer_proc, reinterpret_cast<void*>(i));
        l.push_back(t);
    }
    sleep(3);
    return 0;
}