#include <cstdlib>

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
	BOOST_CHECK_EQUAL(cgi.quantidadeEntradas(), 0);
}

BOOST_AUTO_TEST_CASE(deveInterpretarComNEntradas)
{
	setenv("QUERY_STRING", "teste1=valor1", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.quantidadeEntradas(), 1);
	BOOST_CHECK_EQUAL(cgi["teste1"], "valor1");

	setenv("QUERY_STRING", "teste1=valor1&teste2=valor2&teste3=valor3", 1);

	Cgi cgi2;
	BOOST_CHECK_EQUAL(cgi2.quantidadeEntradas(), 3);
	BOOST_CHECK_EQUAL(cgi2["teste1"], "valor1");
	BOOST_CHECK_EQUAL(cgi2["teste2"], "valor2");
	BOOST_CHECK_EQUAL(cgi2["teste3"], "valor3");
}

BOOST_AUTO_TEST_CASE(deveArmazenarOUltimoValorDeUmaChaveDuplicada)
{
	setenv("QUERY_STRING", "teste1=valor1&teste1=valor2", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.quantidadeEntradas(), 1);
	BOOST_CHECK_EQUAL(cgi["teste1"], "valor2");
}

BOOST_AUTO_TEST_CASE(deveDefinirOMetodoDeAcesso)
{
	setenv("REQUEST_METHOD", "GeT", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getMetodo(), Cgi::Metodo::GET);

	setenv("REQUEST_METHOD", "POsT", 1);

	Cgi cgi2;
	BOOST_CHECK_EQUAL(cgi2.getMetodo(), Cgi::Metodo::POST);

	setenv("REQUEST_METHOD", "ABC", 1);

	Cgi cgi3;
	BOOST_CHECK_EQUAL(cgi3.getMetodo(), Cgi::Metodo::DESCONHECIDO);
}

BOOST_AUTO_TEST_SUITE_END()
