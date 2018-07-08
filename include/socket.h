//
// Created by talen on 2018/7/1.
//

#ifndef UNP_SOCKET_H
#define UNP_SOCKET_H


#include <sys/types.h>
#include <sys/socket.h>
#include <list>
#include <errno.h>
#include "address.h"

#define DEFAULT_BACKLOG 128
#define S_OK 0

using namespace xcom;

void perr_exit(const char *str)
{
		perror(str);
		exit(1);
}
void print(const char *str)
{
		printf("-- %s --\n",str);

}

class Socket
{
public:
int fd;     // the descriptor of socket

public:
Socket() : fd(-1) {
}
~Socket() {
		close();
}

int create(int af, int type, int protocol = 0)
{
		fd = ::socket(af, type, protocol);
		return fd == -1 ? errno : 0;
}

int bind(Address4 addr){
		return ::bind(fd,(struct sockaddr *)&addr,addr.size());
}
int read(char *buf,int len_buf){
		int ret=::read(this->fd,buf,len_buf);
		return ret == -1 ? errno : ret;
}
int write(char *buf,int len_buf){
		int ret=::write(this->fd,buf,len_buf);
		return ret == -1 ? errno : S_OK;
}

void close()
{
		if (fd != -1) {
				// When you have finished using a socket, you can simply close its file descriptor with close
				//#include <unistd.h>
				// ::close(fd);
				// http://www.gnu.org/software/libc/manual/html_node/Closing-a-Socket.html
				::shutdown(fd,2);//2: Stop both reception and transmission.
				fd = -1;
		}
}
};

#define IP_PROTOCOL_TCP 0

class TcpSocket : public Socket
{
public:
int create(int af)
{
		print("create tcp socket");
		return Socket::create(af, SOCK_STREAM, IP_PROTOCOL_TCP);
}
};

class TcpServerSocket : public TcpSocket
{
public:
std::list<int> lst_cfd;    //客户端的文件描述符的列表，维护一个客户端列表
//lst_cfd.push_back(cfd);
//lst_cfd.remove(cfd);


//int print_list(std::list<int> lst){
//    std::list<int>::iterator i;
//    for (i = lst.begin(); i != lst.end(); ++i)
//        std::cout << *i << " ";
//    std::cout<<std::endl;
//}


int listen(Address4 addr)
{
		this->create(AF_INET);
		int ret = this->bind(addr);
		if (ret) return ret;
		else
				print("bind ok");

		ret = ::listen(this->fd, DEFAULT_BACKLOG);
		return ret == -1 ? errno : S_OK;
}
int accept(Address4 &client_addr)
{
		print("wait for conncet");
		socklen_t addr_len = sizeof(client_addr);
		int cfd = ::accept(this->fd,(struct sockaddr *)&client_addr, &addr_len);
		if(cfd == -1)
				perr_exit("accept error");
		return cfd;
}
};

class TcpClientSocket : public TcpSocket
{
public:
int connect(Address4 addr){
		int ret = ::connect(this->fd,(struct sockaddr *)&addr,addr.size());
		return ret == -1 ? errno : S_OK;
}


};


#endif //UNP_SOCKET_H
