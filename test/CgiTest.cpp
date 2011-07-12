/*
This file is part of CGIplus.

CGIplus is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

CGIplus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with CGIplus.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <boost/lexical_cast.hpp>

#include <cgiplus/Cgi.hpp>

using cgiplus::Cgi;

// When you need to run only one test, compile only this file with the
// STAND_ALONE flag.
#ifdef STAND_ALONE
#define BOOST_TEST_MODULE CGIplus
#endif

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(cgiplusTests)

BOOST_AUTO_TEST_CASE(mustParseEmptyInput)
{
	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getNumberOfInputs(), 0);
}

BOOST_AUTO_TEST_CASE(mustParseWithNInputs)
{
	setenv("QUERY_STRING", "key1=value1", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getNumberOfInputs(), 1);
	BOOST_CHECK_EQUAL(cgi["key1"], "value1");

	setenv("QUERY_STRING", "key1=value1&key2=value2&key3=value3", 1);

	Cgi cgi2;
	BOOST_CHECK_EQUAL(cgi2.getNumberOfInputs(), 3);
	BOOST_CHECK_EQUAL(cgi2["key1"], "value1");
	BOOST_CHECK_EQUAL(cgi2["key2"], "value2");
	BOOST_CHECK_EQUAL(cgi2["key3"], "value3");
}

BOOST_AUTO_TEST_CASE(mustCleanOldDataWhenReused)
{
	setenv("QUERY_STRING", "key1=value1", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getNumberOfInputs(), 1);
	BOOST_CHECK_EQUAL(cgi["key1"], "value1");

	setenv("QUERY_STRING", "key2=value2", 1);

	cgi.readInputs();

	BOOST_CHECK_EQUAL(cgi.getNumberOfInputs(), 1);
	BOOST_CHECK_EQUAL(cgi["key2"], "value2");
}

BOOST_AUTO_TEST_CASE(mustStoreTheLastValueOfDuplicatedKey)
{
	setenv("QUERY_STRING", "key1=value1&key1=value2", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getNumberOfInputs(), 1);
	BOOST_CHECK_EQUAL(cgi["key1"], "value2");
}

BOOST_AUTO_TEST_CASE(mustDefineTheAccessMethod)
{
	setenv("REQUEST_METHOD", "GeT", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getMethod(), Cgi::Method::GET);

	setenv("REQUEST_METHOD", "POsT", 1);

	Cgi cgi2;
	BOOST_CHECK_EQUAL(cgi2.getMethod(), Cgi::Method::POST);

	setenv("REQUEST_METHOD", "ABC", 1);

	Cgi cgi3;
	BOOST_CHECK_EQUAL(cgi3.getMethod(), Cgi::Method::UNKNOWN);
}

BOOST_AUTO_TEST_CASE(mustParseDataSentViaPost)
{
	string postInput = "key2=value2";
	string postInputSize = boost::lexical_cast<string>(postInput.size());

	setenv("QUERY_STRING", "key1=value1", 1);
	setenv("CONTENT_LENGTH", postInputSize.c_str(), 1);
	setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
	setenv("REQUEST_METHOD", "POST", 1);

	std::cin.clear();
	for (auto it = postInput.rbegin(); it != postInput.rend(); it++) {
		std::cin.putback(*it);
	}

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getNumberOfInputs(), 2);
	BOOST_CHECK_EQUAL(cgi["key1"], "value1");
	BOOST_CHECK_EQUAL(cgi["key2"], "value2");
}

BOOST_AUTO_TEST_CASE(mustDecodeAnUrlCorrectly)
{
	setenv("REQUEST_METHOD", "GET", 1);
	setenv("QUERY_STRING", "key1=value1+value2+%2B%3A", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getNumberOfInputs(), 1);
	BOOST_CHECK_EQUAL(cgi["key1"], "value1 value2 +:");
}

BOOST_AUTO_TEST_CASE(mustParseCookies)
{
	setenv("REQUEST_METHOD", "GET", 1);
	setenv("HTTP_COOKIE", "key1=value1; key2=value2; key3=value3", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getNumberOfCookies(), 3);
	BOOST_CHECK_EQUAL(cgi("key1"), "value1");
	BOOST_CHECK_EQUAL(cgi("key2"), "value2");
	BOOST_CHECK_EQUAL(cgi("key3"), "value3");
}

BOOST_AUTO_TEST_SUITE_END()
