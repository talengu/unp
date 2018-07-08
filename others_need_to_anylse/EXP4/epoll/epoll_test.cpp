#include  <stdio.h> 
#include  <iostream> 
#include  <unistd.h> 
#include  <fcntl.h> 
#include  <errno.h>
#include  <sys/types.h>
#include  <sys/socket.h> 
#include  <sys/epoll.h> 
#include  <netinet/in.h> 
#include  <arpa/inet.h> 
#include <epoll_server.h>


int main(int argc, char *argv[])
{
	epoll_server my_epoll;
	my_epoll.init(12345, 10);
	//my_epoll.init("127.0.0.1", 12345, 10);
	my_epoll.run(2000);
    return  0;
}