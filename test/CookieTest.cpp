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
