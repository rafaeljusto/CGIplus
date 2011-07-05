#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include <cgiplus/Cgi.H>

using cgiplus::Cgi;

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE Cgi

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(cgiplusTests)

BOOST_AUTO_TEST_CASE(deveInterpretarEntradaVazia)
{
	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.numberOfInputs(), 0);
}

BOOST_AUTO_TEST_CASE(deveInterpretarComNEntradas)
{
	setenv("QUERY_STRING", "teste1=valor1", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.numberOfInputs(), 1);
	BOOST_CHECK_EQUAL(cgi["teste1"], "valor1");

	setenv("QUERY_STRING", "teste1=valor1&teste2=valor2&teste3=valor3", 1);

	Cgi cgi2;
	BOOST_CHECK_EQUAL(cgi2.numberOfInputs(), 3);
	BOOST_CHECK_EQUAL(cgi2["teste1"], "valor1");
	BOOST_CHECK_EQUAL(cgi2["teste2"], "valor2");
	BOOST_CHECK_EQUAL(cgi2["teste3"], "valor3");
}

BOOST_AUTO_TEST_CASE(deveArmazenarOUltimoValorDeUmaChaveDuplicada)
{
	setenv("QUERY_STRING", "teste1=valor1&teste1=valor2", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.numberOfInputs(), 1);
	BOOST_CHECK_EQUAL(cgi["teste1"], "valor2");
}

BOOST_AUTO_TEST_CASE(deveDefinirOMethodDeAcesso)
{
	setenv("REQUEST_METHOD", "GeT", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.method(), Cgi::Method::GET);

	setenv("REQUEST_METHOD", "POsT", 1);

	Cgi cgi2;
	BOOST_CHECK_EQUAL(cgi2.method(), Cgi::Method::POST);

	setenv("REQUEST_METHOD", "ABC", 1);

	Cgi cgi3;
	BOOST_CHECK_EQUAL(cgi3.method(), Cgi::Method::UNKNOWN);
}

BOOST_AUTO_TEST_CASE(deveInterpretarDadosEnviadosViaPost)
{
	string entradaPost = "teste2=valor2";
	string tamanhoEntradaPost = 
		boost::lexical_cast<string>(entradaPost.size());

	setenv("QUERY_STRING", "teste1=valor1", 1);
	setenv("CONTENT_LENGTH", tamanhoEntradaPost.c_str(), 1);
	setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
	setenv("REQUEST_METHOD", "POST", 1);

	std::cin.clear();
	for (auto it = entradaPost.rbegin(); it != entradaPost.rend(); it++) {
		std::cin.putback(*it);
	}

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.numberOfInputs(), 2);
	BOOST_CHECK_EQUAL(cgi["teste1"], "valor1");
	BOOST_CHECK_EQUAL(cgi["teste2"], "valor2");
}

BOOST_AUTO_TEST_CASE(deveDecodificarCorretamenteUmaUrl)
{
	setenv("REQUEST_METHOD", "GET", 1);
	setenv("QUERY_STRING", "teste1=valor1+valor2+%2B%3A", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.numberOfInputs(), 1);
	BOOST_CHECK_EQUAL(cgi["teste1"], "valor1 valor2 +:");
}

BOOST_AUTO_TEST_SUITE_END()
