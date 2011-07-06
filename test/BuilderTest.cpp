#include <cgiplus/Builder.hpp>

using cgiplus::Builder;

// When you need to run only one test, compile only this file with the
// STAND_ALONE flag.
#ifdef STAND_ALONE
#define BOOST_TEST_MODULE CGIplus
#endif

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(cgiplusTests)

BOOST_AUTO_TEST_CASE(deveSubstituirCorretamenteNoTemplate)
{
	string form = "<html><body><!-- test --></body></html>";

	Builder builder1;
	builder1.setForm(form);

	string content1 = "Content-type: text/html\n\r\n\r" + form;
	BOOST_CHECK_EQUAL(builder1.build(), content1);

	Builder builder2;
	builder2.setForm(form);
	builder2["test"] = "This is a test";

	string content2 = "Content-type: text/html\n\r\n\r"
		"<html><body>This is a test</body></html>";
	BOOST_CHECK_EQUAL(builder2.build(), content2);
}

BOOST_AUTO_TEST_SUITE_END()
