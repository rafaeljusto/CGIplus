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

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include <boost/lexical_cast.hpp>

#include <cgiplus/Cgi.hpp>
#include <cgiplus/Charset.hpp>
#include <cgiplus/HttpHeader.hpp>
#include <cgiplus/Language.hpp>
#include <cgiplus/MediaType.hpp>

using cgiplus::Cgi;
using cgiplus::Charset;
using cgiplus::HttpHeader;
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

BOOST_AUTO_TEST_CASE(mustParseEmptyInput)
{
	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getNumberOfInputs(), 0);
}

BOOST_AUTO_TEST_CASE(mustParseWithNInputs)
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

BOOST_AUTO_TEST_CASE(mustCleanOldDataWhenReused)
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

BOOST_AUTO_TEST_CASE(mustStoreTheLastValueOfDuplicatedKey)
{
	setenv("QUERY_STRING", "key1=value1&key1=value2", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getNumberOfInputs(), 1);
	BOOST_CHECK_EQUAL(cgi["key1"], "value2");
}

BOOST_AUTO_TEST_CASE(mustDefineTheAccessMethod)
{
	setenv("REQUEST_METHOD", "GeT", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi->getMethod(), HttpHeader::Method::GET);

	setenv("REQUEST_METHOD", "POsT", 1);

	Cgi cgi2;
	BOOST_CHECK_EQUAL(cgi2->getMethod(), HttpHeader::Method::POST);

	setenv("REQUEST_METHOD", "ABC", 1);

	Cgi cgi3;
	BOOST_CHECK_EQUAL(cgi3->getMethod(), HttpHeader::Method::UNDEFINED);
}

BOOST_AUTO_TEST_CASE(mustParseDataSentViaPost)
{
	string postInput = "key2=value2";
	string postInputSize = boost::lexical_cast<string>(postInput.size());

	setenv("QUERY_STRING", "key1=value1", 1);
	setenv("CONTENT_LENGTH", postInputSize.c_str(), 1);
	setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
	setenv("REQUEST_METHOD", "POST", 1);

	std::cin.clear();
	for (auto it = postInput.rbegin(); it != postInput.rend(); it++) {
		std::cin.putback(*it);
	}

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getNumberOfInputs(), 2);
	BOOST_CHECK_EQUAL(cgi["key1"], "value1");
	BOOST_CHECK_EQUAL(cgi["key2"], "value2");
}

BOOST_AUTO_TEST_CASE(mustDecodeAnUrlCorrectly)
{
	setenv("REQUEST_METHOD", "GET", 1);
	setenv("QUERY_STRING", "key1=value1+value2+%2B%3A", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getNumberOfInputs(), 1);
	BOOST_CHECK_EQUAL(cgi["key1"], "value1 value2 +:");
}

BOOST_AUTO_TEST_CASE(mustParseCookies)
{
	setenv("REQUEST_METHOD", "GET", 1);
	setenv("HTTP_COOKIE", "key1=value1; key2=value2; key3=value3", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getNumberOfCookies(), 3);
	BOOST_CHECK_EQUAL(cgi("key1"), "value1");
	BOOST_CHECK_EQUAL(cgi("key2"), "value2");
	BOOST_CHECK_EQUAL(cgi("key3"), "value3");
}

BOOST_AUTO_TEST_CASE(mustParseRemoteAddress)
{
	setenv("REMOTE_ADDR", "127.0.0.1", 1);
	setenv("REQUEST_METHOD", "GET", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getRemoteAddress(), "127.0.0.1");

	unsetenv("REMOTE_ADDR");
	
	Cgi cgi2;
	BOOST_CHECK_EQUAL(cgi2.getRemoteAddress(), "");
}

BOOST_AUTO_TEST_CASE(mustConvertInputData)
{
	setenv("REQUEST_METHOD", "GET", 1);
	setenv("QUERY_STRING", "key1=5&key2=5.1", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.get<int>("key1"), 5);
	BOOST_CHECK_EQUAL(cgi.get<double>("key2"), 5.1);
}

BOOST_AUTO_TEST_CASE(mustConvertInputDataIntoObject)
{
	setenv("REQUEST_METHOD", "GET", 1);
	setenv("QUERY_STRING", "name=Rafael&age=26", 1);

	class Person
	{
	public:
		Person() :
			name(""),
			age(0)
		{}

		string name;
		unsigned int age;
	};

	Cgi cgi;
	Person person = cgi.get<Person>
		(Cgi::Source::FIELD, 
		 [] (std::map<string, string> fields)
		 {
			 Person person;
			 person.name = fields["name"];
			 person.age = boost::lexical_cast<unsigned int>(fields["age"]);
			 return person;
		 });

	BOOST_CHECK_EQUAL(person.name, "Rafael");
	BOOST_CHECK_EQUAL(person.age, 26);
}

BOOST_AUTO_TEST_CASE(mustParseUploadedFile)
{
	string content = "multipart/form-data; boundary=AaB03x";

	string file = "--AaB03x\n"
		"Content-Disposition: form-data; name=\"file\"; filename=\"file.txt\"\n"
		"Content-Type: text/plain\n\n"
		"... contents of file.txt ...\n"
		"--AaB03x--\n";
	string fileSize = boost::lexical_cast<string>(file.size());

	setenv("CONTENT_TYPE", content.c_str(), 1);
	setenv("CONTENT_LENGTH", fileSize.c_str(), 1);
	setenv("REQUEST_METHOD", "POST", 1);

	std::cin.clear();
	for (auto it = file.rbegin(); it != file.rend(); it++) {
		std::cin.putback(*it);
	}

	Cgi cgi;

	string fileLine("");
	std::ifstream fileStream(cgi.get("file", Cgi::Source::FILE));
	BOOST_CHECK_EQUAL(fileStream.good(), true);

	std::getline(fileStream, fileLine);
	BOOST_CHECK_EQUAL(fileLine, "... contents of file.txt ...");
	fileStream.close();
}

BOOST_AUTO_TEST_CASE(mustParseAcceptField)
{
	setenv("REQUEST_METHOD", "GET", 1);
	setenv("HTTP_ACCEPT", "application/json", 1);

	Cgi cgi;

	BOOST_CHECK_EQUAL(cgi->getAccepts().size(), 1);
	if (cgi->getAccepts().empty() == false) {
		BOOST_CHECK_EQUAL(*cgi->getAccepts().begin(), MediaType::APPLICATION_JSON);
	}
}

BOOST_AUTO_TEST_CASE(mustBuildURIFields) {
	setenv("SCRIPT_NAME", "/cgi-bin/cgi", 1);
	setenv("PATH_INFO", "/param/1", 1);

	Cgi cgi;
	BOOST_CHECK_EQUAL(cgi.getURI(), "/cgi-bin/cgi/param/1");
}

BOOST_AUTO_TEST_CASE(mustParseLanguageField) {
	setenv("REQUEST_METHOD", "GET", 1);
	setenv("CONTENT_LANGUAGE", "pt-BR, en-US", 1);

	Cgi cgi;

	std::set<Language::Value> languages = cgi->getContentLanguages();
	BOOST_CHECK_EQUAL(languages.size(), 2);

	if (languages.empty() == false) {
		BOOST_CHECK(languages.find(Language::PORTUGUESE_BR) != languages.end());
		BOOST_CHECK(languages.find(Language::ENGLISH_US) != languages.end());
	}
}

BOOST_AUTO_TEST_CASE(mustParseResponseLanguageField) {
	setenv("REQUEST_METHOD", "GET", 1);
	setenv("HTTP_ACCEPT_LANGUAGE", "pt, en-US;q=0.8, en;q=0.7", 1);

	Cgi cgi;

	std::vector<Language::Value> languages = cgi->getAcceptLanguages();
	BOOST_CHECK_EQUAL(languages.size(), 3);

	if (languages.empty() == false) {
		BOOST_CHECK(std::find(languages.begin(), 
		                      languages.end(), 
		                      Language::PORTUGUESE_ANY) != languages.end());
		BOOST_CHECK(std::find(languages.begin(), 
		                      languages.end(), 
		                      Language::ENGLISH_US) != languages.end());
		BOOST_CHECK(std::find(languages.begin(), 
		                      languages.end(), 
		                      Language::ENGLISH_ANY) != languages.end());
	}
}

BOOST_AUTO_TEST_CASE(mustRespectResponseLanguageQuality) {
	setenv("REQUEST_METHOD", "GET", 1);
	setenv("HTTP_ACCEPT_LANGUAGE", "pt;q=0.2, en-US;q=0.3, en;q=0.8", 1);

	Cgi cgi;

	std::vector<Language::Value> languages = cgi->getAcceptLanguages();
	BOOST_CHECK_EQUAL(languages.size(), 3);

	if (languages.empty() == false) {
		BOOST_CHECK_EQUAL(languages[0], Language::ENGLISH_ANY);
		BOOST_CHECK_EQUAL(languages[1], Language::ENGLISH_US);
		BOOST_CHECK_EQUAL(languages[2], Language::PORTUGUESE_ANY);
	}
}

BOOST_AUTO_TEST_CASE(mustParseEncoding) {
	setenv("CONTENT_TYPE", "application/x-www-form-urlencoded; charset=utf-8", 1);

	Cgi cgi;

	BOOST_CHECK_EQUAL(cgi->getContentCharset(), Charset::UTF8);
}

BOOST_AUTO_TEST_CASE(mustParseResponseEncoding) {
	setenv("HTTP_ACCEPT_CHARSET", "utf-8, iso-8859-1", 1);

	Cgi cgi;

	std::vector<Charset::Value> encodings = cgi->getAcceptCharsets();
	BOOST_CHECK_EQUAL(encodings.size(), 2);

	if (encodings.empty() == false) {
		BOOST_CHECK(std::find(encodings.begin(), 
		                      encodings.end(), 
		                      Charset::UTF8) != encodings.end());
		BOOST_CHECK(std::find(encodings.begin(), 
		                      encodings.end(), 
		                      Charset::ISO88591) != encodings.end());
	}
}

BOOST_AUTO_TEST_CASE(mustRespectResponseEncodingQuality) {
	setenv("HTTP_ACCEPT_CHARSET", "utf-8;q=0.6,iso-8859-1;q=0.9,utf-16;q=0.5",1);

	Cgi cgi;

	std::vector<Charset::Value> encodings = cgi->getAcceptCharsets();
	BOOST_CHECK_EQUAL(encodings.size(), 3);

	if (encodings.empty() == false) {
		BOOST_CHECK_EQUAL(encodings[0], Charset::ISO88591);
		BOOST_CHECK_EQUAL(encodings[1], Charset::UTF8);
		BOOST_CHECK_EQUAL(encodings[2], Charset::UTF16);
	}
}

BOOST_AUTO_TEST_SUITE_END()
