/**
 * @file socketTest.cc
 * @comment
 * implement the test cases declared in socketTest.h
 *
 * @author niexw
 * @email xiaowen.nie.cn@gmail.com
 */
 
#include <boost/test/test_tools.hpp>
#include "socketTest.h"
#include "socket.h"

void SocketTestCases::testCreate()
{
    Socket sock;
    BOOST_CHECK(sock.fd == -1);

    int ret = sock.create(AF_INET, SOCK_STREAM, 0);
    BOOST_CHECK(ret == 0);
    BOOST_CHECK(sock.fd != -1);

    Socket sock1;
    ret = sock1.create(AF_INET, SOCK_DGRAM);
    BOOST_CHECK(ret == 0);
    BOOST_CHECK(sock1.fd != -1);
    BOOST_CHECK(sock1.fd != fd);
}

