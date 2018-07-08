#include <iostream>
#include <pthread.h>
#include "thread.h"

using namespace std;

int main(int argv,char *argc)
{
	MultiThread tt;
	tt.start();
	tt.join();
	return 0;
}
