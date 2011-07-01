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
	BOOST_CHECK_EQUAL(cgi.getMetodo(), Cgi::Metodo::DESCONHECIDO);
}

BOOST_AUTO_TEST_SUITE_END()
