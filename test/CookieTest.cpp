#include <cgiplus/Cookie.hpp>

using cgiplus::Cookie;

// When you need to run only one test, compile only this file with the
// STAND_ALONE flag.
#ifdef STAND_ALONE
#define BOOST_TEST_MODULE CGIplus
#endif

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(cgiplusTests)

BOOST_AUTO_TEST_CASE(deveGerarUmCookieCorretamente)
{
	Cookie cookie;
	cookie
		.setDomain("test.com")
		.setPath("/")
		.setKey("key")
		.setValue("value")
		.setSecure(true)
		.setHttpOnly(true);

	string request = "Set-Cookie: key=value; "
		"Domain=test.com; Path=/; Secure; httponly; ";
	BOOST_CHECK_EQUAL(cookie.build(), request);
}

BOOST_AUTO_TEST_SUITE_END()
