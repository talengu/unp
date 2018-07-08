/**
 * @file suites.h
 * @comment
 * define all test suites here
 *
 * @author niexw
 * @email xiaowen.nie.cn@gmail.com
 */

#ifndef _TEST_SUITES_H_
#define _TEST_SUITES_H_

#include "LogTest.h"
#include "ExceptionTest.h"
#include "SocketTest.h"

class LogTestSuite : public test_suite
{
public:
LogTestSuite() : test_suite("LogTestSuite")
{
		boost::shared_ptr<LogTestCases> instance(new LogTestCases());
		add(BOOST_CLASS_TEST_CASE(&LogTestCases::testLog, instance));
}
};

class ExceptionTestSuite : public test_suite
{
public:
ExceptionTestSuite() : test_suite("ExceptionTestSuite")
{
		boost::shared_ptr<ExceptionTestCases> instance(new ExceptionTestCases());
		add(BOOST_CLASS_TEST_CASE(&ExceptionTestCases::testException, instance));
}
};

class SocketTestSuite : public test_suite
{
public:
SocketTestSuite() : test_suite("SocketTestSuite")
{
		boost::shared_ptr<SocketTestCases> instance(new SocketTestCases());
		add(BOOST_CLASS_TEST_CASE(&SocketTestCases::testCreate, instance));
		add(BOOST_CLASS_TEST_CASE(&SocketTestCases::testTcpServerandClient, instance));
}
};

#endif // _TEST_SUITES_H_
