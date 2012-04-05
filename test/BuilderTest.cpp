/*
  CGIplus Copyright (C) 2012 Rafael Dantas Justo

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
#include <fstream>

#include <boost/lexical_cast.hpp>

#include <cgiplus/Builder.hpp>
#include <cgiplus/Cookie.hpp>
#include <cgiplus/Language.hpp>
#include <cgiplus/MediaType.hpp>

using boost::lexical_cast;

using cgiplus::Builder;
using cgiplus::Cookie;
using cgiplus::Language;
using cgiplus::MediaType;

// When you need to run only one test, compile only this file with the
// STAND_ALONE flag.
#ifdef STAND_ALONE
#define BOOST_TEST_MODULE CGIplus
#endif

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(cgiplusTests)

BOOST_AUTO_TEST_CASE(mustNotReplaceWhenThereIsNoKeyNoMatch)
{
	string form = "<html><body><!-- test --></body></html>";

	Builder builder;
	builder.setForm(form);

	string content = "Content-Type: text/html" + Builder::EOL +
		"Content-Length: 39" + Builder::EOL +
		"Content-Language: en-US" + Builder::EOL + Builder::EOL + form;
	BOOST_CHECK_EQUAL(builder.build(), content);
}

BOOST_AUTO_TEST_CASE(mustReplaceWhenThereIsAKeyMatch)
{
	string form1 = "<html><body><!-- test --></body></html>";

	Builder builder1;
	builder1.setForm(form1);
	builder1["test"] = "This is a test.";

	string content1 = "Content-Type: text/html" + Builder::EOL +
		"Content-Length: 41" + Builder::EOL +
		"Content-Language: en-US" + Builder::EOL + Builder::EOL +
		"<html><body>This is a test.</body></html>";
	BOOST_CHECK_EQUAL(builder1.build(), content1);

	string form2 = "<html><body><!-- test1 --> <!-- test2 --> "
		"<!-- test3 --> <!-- test4 --></body></html>";

	Builder builder2;
	builder2.setForm(form2);
	builder2["test1"] = "This is a test.";
	builder2["test3"] = "Another test.";
	builder2["test4"] = "Guess what? One more test!";

	string content2 = "Content-Type: text/html" + Builder::EOL +
		"Content-Length: 97" + Builder::EOL +
		"Content-Language: en-US" + Builder::EOL + Builder::EOL +
		"<html><body>This is a test. <!-- test2 --> Another test. "
		"Guess what? One more test!</body></html>";
	BOOST_CHECK_EQUAL(builder2.build(), content2);
}

BOOST_AUTO_TEST_CASE(mustDefineCookieCorrectly)
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

	string content = "Content-Type: text/html" + Builder::EOL +
		"Content-Length: 30" + Builder::EOL +
		"Content-Language: en-US" + Builder::EOL +
		"Set-Cookie: key=value; Domain=test.com.br; Path=/; Secure; httponly; " + 
		Builder::EOL + Builder::EOL +
		"<html><body>Test</body></html>";
	BOOST_CHECK_EQUAL(builder.build(), content);
}

BOOST_AUTO_TEST_CASE(mustLoadATemplateFileCorrectly)
{
	std::ofstream templateFile("template-file.tmp");
	templateFile << "This is a <!-- test -->";
	templateFile.close();

	Builder builder;
	builder.setFormFile("template-file.tmp");
	builder["test"] = "test";

	string content = "Content-Type: text/html" + Builder::EOL +
		"Content-Length: 14" + Builder::EOL +
		"Content-Language: en-US" + Builder::EOL + Builder::EOL +
		"This is a test";
	BOOST_CHECK_EQUAL(builder.build(), content);

	remove("template-file.tmp");
}

BOOST_AUTO_TEST_CASE(mustFlushTemplateWhenTemplateFileWasNotFound)
{
	Builder builder;
	builder.setForm("Any form example");
	builder.setFormFile("idontexist.tmp");
	builder["test"] = "test";

	string content = "Content-Type: text/html" + Builder::EOL +
		"Content-Length: 0" + Builder::EOL +
		"Content-Language: en-US" + Builder::EOL + Builder::EOL;
	BOOST_CHECK_EQUAL(builder.build(), content);
}

BOOST_AUTO_TEST_CASE(mustNotFlushOldDataWhenReused)
{
	string form = "<html><body><!-- test1 --> <!-- test2 --> "
		"<!-- test3 --> <!-- test4 --></body></html>";

	Builder builder;
	builder["test1"] = "This is a test.";

	builder.setForm(form);
	builder["test2"] = "This is a test.";
	builder["test3"] = "Another test.";
	builder["test4"] = "Guess what? One more test!";

	string content = "Content-Type: text/html" + Builder::EOL +
		"Content-Length: 98" + Builder::EOL +
		"Content-Language: en-US" + Builder::EOL + Builder::EOL +
		"<html><body>This is a test. This is a test. Another test. "
		"Guess what? One more test!</body></html>";

	BOOST_CHECK_EQUAL(builder.build(), content);

	std::ofstream templateFile("template-file.tmp");
	templateFile << form;
	templateFile.close();
	builder.setFormFile("template-file.tmp");

	BOOST_CHECK_EQUAL(builder.build(), content);

	remove("template-file.tmp");
}

BOOST_AUTO_TEST_CASE(mustBuildRedirectCorrectly)
{
	string redirection = 
		"Location: http://127.0.0.1" + Builder::EOL + Builder::EOL;

	Builder builder;
	BOOST_CHECK_EQUAL(builder.redirect("http://127.0.0.1"), redirection);
}

BOOST_AUTO_TEST_CASE(mustBuildStatusCorrectly)
{
	Builder builder;
	builder.setStatus(Builder::Status::OK, "Test");

	string content = "Status: 200 Test" + Builder::EOL +
		"Content-Type: text/html" + Builder::EOL +
		"Content-Length: 0" + Builder::EOL +
		"Content-Language: en-US" + Builder::EOL + Builder::EOL;

	BOOST_CHECK_EQUAL(builder.build(), content);
}

BOOST_AUTO_TEST_CASE(mustChangeFormat)
{
	Builder builder;
	builder.setFormat(MediaType::APPLICATION_JSON);

	string content = "Content-Type: application/json" + Builder::EOL +
		"Content-Length: 0" + Builder::EOL +
		"Content-Language: en-US" + Builder::EOL + Builder::EOL;

	BOOST_CHECK_EQUAL(builder.build(), content);
}

BOOST_AUTO_TEST_CASE(mustChangeLanguage)
{
	Builder builder;
	builder.setLanguage(Language::PORTUGUESE_BR);

	string content = "Content-Type: text/html" + Builder::EOL +
		"Content-Length: 0" + Builder::EOL +
		"Content-Language: pt-BR" + Builder::EOL + Builder::EOL;

	BOOST_CHECK_EQUAL(builder.build(), content);
}

BOOST_AUTO_TEST_SUITE_END()
