/**
 * @file socketTest.cc
 * @comment
 * implement the test cases declared in socketTest.h
 *
 * @author niexw
 * @email xiaowen.nie.cn@gmail.com
 */

#include <boost/test/test_tools.hpp>
#include "SocketTest.h"
#include "socket.h"
#include <pthread.h>
#include <iostream>

void SocketTestCases::testCreate()
{
		Socket sock;
		BOOST_CHECK(sock.fd == -1);

		int ret = sock.create(AF_INET, SOCK_STREAM, 0);
		BOOST_CHECK(ret == 0);
		BOOST_CHECK(sock.fd != -1);

}

void* tcp_server(void *arg){
		TcpServerSocket ts;
		int ret =ts.listen(Address4(9988));
		BOOST_CHECK(ret == 0);

		Address4 client_addr;
		// int cfd=ts.accept(client_addr);
		// std::cout<<client_addr<<std::endl;
		// BOOST_CHECK(cfd != -1);
		return((void*)0);
}
void openserver(){
		pthread_t th;
		pthread_create( &th, NULL, tcp_server, NULL);
		pthread_join(th,NULL);
}

void SocketTestCases::testTcpServerandClient()
{
		openserver();
		TcpClientSocket tc;
		tc.create(AF_INET);
		tc.connect(Address4(9988));

}
