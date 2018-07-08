/**
 * @file socketTest.h
 * @comment
 * declare the test cases of Socket
 *
 * @author niexw
 * @email xiaowen.nie.cn@gmail.com
 */
#ifndef __UNP_SOCET_TEST_H__
#define __UNP_SOCET_TEST_H__

class SocketTestCases
{
public:
void testCreate();
void testTcpServer();
void testTcpServerandClient();
};

// 开启 server
void* tcp_server(void *);
void openserver();

#endif // __UNP_SOCKET_TEST_H__
