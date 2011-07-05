#include <cgiplus/Builder.H>

using cgiplus::Builder;

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE Builder

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(cgiplusTests)

BOOST_AUTO_TEST_CASE(deveSubstituirCorretamenteNoTemplate)
{
	string form = "<html><body><!-- test --></body></html>";

	Builder builder;
	builder.setForm(form);
	builder["test"] = "This is a test";

	string content = "Content-type: text/html\n\r\n\r"
		"<html><body>This is a test</body></html>";
	BOOST_CHECK_EQUAL(builder.build(), content);
}

BOOST_AUTO_TEST_SUITE_END()
