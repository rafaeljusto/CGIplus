#include <cstdio>
#include <fstream>

#include <cgiplus/Builder.hpp>
#include <cgiplus/Cookie.hpp>

using cgiplus::Builder;
using cgiplus::Cookie;

// When you need to run only one test, compile only this file with the
// STAND_ALONE flag.
#ifdef STAND_ALONE
#define BOOST_TEST_MODULE CGIplus
#endif

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(cgiplusTests)

BOOST_AUTO_TEST_CASE(naoDeveSubstituirNadaQuandoVazio)
{
	string form = "<html><body><!-- test --></body></html>";

	Builder builder;
	builder.setForm(form);

	string content = "Content-type: text/html\n\r\n\r" + form;
	BOOST_CHECK_EQUAL(builder.build(), content);
}

BOOST_AUTO_TEST_CASE(deveSubstituirCorretamenteNoTemplate)
{
	string form1 = "<html><body><!-- test --></body></html>";

	Builder builder1;
	builder1.setForm(form1);
	builder1["test"] = "This is a test.";

	string content1 = "Content-type: text/html\n\r\n\r"
		"<html><body>This is a test.</body></html>";
	BOOST_CHECK_EQUAL(builder1.build(), content1);

	string form2 = "<html><body><!-- test1 --> <!-- test2 --> "
		"<!-- test3 --> <!-- test4 --></body></html>";

	Builder builder2;
	builder2.setForm(form2);
	builder2["test1"] = "This is a test.";
	builder2["test3"] = "Another test.";
	builder2["test4"] = "Guess what? One more test!";

	string content2 = "Content-type: text/html\n\r\n\r"
		"<html><body>This is a test. <!-- test2 --> Another test. "
		"Guess what? One more test!</body></html>";
	BOOST_CHECK_EQUAL(builder2.build(), content2);
}

BOOST_AUTO_TEST_CASE(deveDefinirCorretamenteUmCookie) 
{
	string form = "<html><body>Test</body></html>";

	Builder builder;
	builder.setForm(form);
	
	builder("key")
		.setValue("value")
		.setDomain("test.com.br")
		.setPath("/")
		.setSecure(true)
		.setHttpOnly(true);

	string content = "Content-type: text/html\n\r\n\r"
		"Set-Cookie: key=value; Domain=test.com.br; Path=/; Secure; httponly; "
		"<html><body>Test</body></html>";
	BOOST_CHECK_EQUAL(builder.build(), content);
}

BOOST_AUTO_TEST_CASE(deveCarregarCorretamenteUmArquivoTemplate) 
{
	std::ofstream templateFile("template-file.tmp");
	templateFile << "This is a <!-- test -->" << std::endl;
	templateFile.close();

	Builder builder;
	builder.setFormFile("template-file.tmp");
	builder["test"] = "test";
	
	string content = "Content-type: text/html\n\r\n\r"
		"This is a test";
	BOOST_CHECK_EQUAL(builder.build(), content);
	
	remove("template-file.tmp");
}

BOOST_AUTO_TEST_CASE(naoDeveArmazenarDadosAoSerReaproveitado) 
{
	string form1 = "<html><body><!-- test1 --></body></html>";

	Builder builder;
	builder.setForm(form1);
	builder["test1"] = "This is a test.";

	string content1 = "Content-type: text/html\n\r\n\r"
		"<html><body>This is a test.</body></html>";
	BOOST_CHECK_EQUAL(builder.build(), content1);

	string form2 = "<html><body><!-- test1 --> <!-- test2 --> "
		"<!-- test3 --> <!-- test4 --></body></html>";

	builder.setForm(form2);
	builder["test2"] = "This is a test.";
	builder["test3"] = "Another test.";
	builder["test4"] = "Guess what? One more test!";

	string content2 = "Content-type: text/html\n\r\n\r"
		"<html><body><!-- test1 --> This is a test. Another test. "
		"Guess what? One more test!</body></html>";
	BOOST_CHECK_EQUAL(builder.build(), content2);
}

BOOST_AUTO_TEST_SUITE_END()
