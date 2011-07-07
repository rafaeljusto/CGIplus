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

BOOST_AUTO_TEST_CASE(deveInterpretarEntradaVazia)
{
	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getNumberOfInputs(), 0);
}

BOOST_AUTO_TEST_CASE(deveInterpretarComNEntradas)
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

BOOST_AUTO_TEST_CASE(deveLimparOsDadosAntigosQuandoForReaproveitado)
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

BOOST_AUTO_TEST_CASE(deveArmazenarOUltimoValueDeUmaChaveDuplicada)
{
	setenv("QUERY_STRING", "key1=value1&key1=value2", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getNumberOfInputs(), 1);
	BOOST_CHECK_EQUAL(cgi["key1"], "value2");
}

BOOST_AUTO_TEST_CASE(deveDefinirOMetodoDeAcesso)
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

BOOST_AUTO_TEST_CASE(deveInterpretarDadosEnviadosViaPost)
{
	string entradaPost = "key2=value2";
	string tamanhoEntradaPost = 
		boost::lexical_cast<string>(entradaPost.size());

	setenv("QUERY_STRING", "key1=value1", 1);
	setenv("CONTENT_LENGTH", tamanhoEntradaPost.c_str(), 1);
	setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
	setenv("REQUEST_METHOD", "POST", 1);

	std::cin.clear();
	for (auto it = entradaPost.rbegin(); it != entradaPost.rend(); it++) {
		std::cin.putback(*it);
	}

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getNumberOfInputs(), 2);
	BOOST_CHECK_EQUAL(cgi["key1"], "value1");
	BOOST_CHECK_EQUAL(cgi["key2"], "value2");
}

BOOST_AUTO_TEST_CASE(deveDecodificarCorretamenteUmaUrl)
{
	setenv("REQUEST_METHOD", "GET", 1);
	setenv("QUERY_STRING", "key1=value1+value2+%2B%3A", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getNumberOfInputs(), 1);
	BOOST_CHECK_EQUAL(cgi["key1"], "value1 value2 +:");
}

BOOST_AUTO_TEST_CASE(deveInterpretarCookies)
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
