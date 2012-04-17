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

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/regex.hpp>

#include <cgiplus/Cgi.hpp>
#include <cgiplus/UploadedFile.hpp>

CGIPLUS_NS_BEGIN

Cgi::Cgi() :
	_uri(""),
	_remoteAddress("")
{
	readInputs();
}

Cgi::~Cgi()
{
	for (auto file: _files) {
		if (remove(file.second.c_str()) == -1) {
			// Error while trying to remove file, leave the file there
		}
	}
}

string Cgi::operator[](const string &key) const
{
	return get(key);
}

string Cgi::operator()(const string &key) const
{
	return get(key, Source::COOKIE);
}

HttpHeader const* Cgi::operator->() const
{
	return &_httpHeader;
}

void Cgi::readInputs()
{
	clearInputs();
	readMethod();
	readContentType();
	readQueryStringInputs();
	readContentInputs();
	readContentLanguages();
	readAccepts();
	readAcceptLanguages();
	readAcceptCharsets();
	readCookies();
	readURI();
	readRemoteAddress();
}

unsigned int Cgi::getNumberOfInputs() const
{
	return _inputs.size();
}

unsigned int Cgi::getNumberOfCookies() const
{
	return _httpHeader.getCookies().size();
}

string Cgi::getURI() const
{
	return _uri;
}

string Cgi::getRemoteAddress() const
{
	return _remoteAddress;
}

void Cgi::clearInputs()
{
	_httpHeader.clear();
	_inputs.clear();
	_files.clear();
	_uri.clear();
	_remoteAddress.clear();
}

void Cgi::readMethod()
{
	const char *methodPtr = getenv("REQUEST_METHOD");
	if (methodPtr != NULL) {
		string method = boost::to_upper_copy((string) methodPtr);
		if (method == "CONNECT") {
			_httpHeader.setMethod(HttpHeader::Method::CONNECT);
		} else if (method == "DELETE") {
			_httpHeader.setMethod(HttpHeader::Method::DELETE);
		} else if (method == "HEAD") {
			_httpHeader.setMethod(HttpHeader::Method::HEAD);
		} else if (method == "GET") {
			_httpHeader.setMethod(HttpHeader::Method::GET);
		} else if (method == "OPTIONS") {
			_httpHeader.setMethod(HttpHeader::Method::OPTIONS);
		} else if (method == "PATCH") {
			_httpHeader.setMethod(HttpHeader::Method::PATCH);
		} else if (method == "POST") {
			_httpHeader.setMethod(HttpHeader::Method::POST);
		} else if (method == "PUT") {
			_httpHeader.setMethod(HttpHeader::Method::PUT);
		} else if (method == "TRACE") {
			_httpHeader.setMethod(HttpHeader::Method::TRACE);
		}
	}
}

void Cgi::readContentType()
{
	const char *typePtr = getenv("CONTENT_TYPE");
	if (typePtr == NULL) {
		return;
	}

	string type = typePtr;

	std::vector<string> typeItems;
	boost::split(typeItems, type, boost::is_any_of(";"));

	if (typeItems.empty()) {
		return;
	}

	_httpHeader.setContentType(MediaType::detect(typeItems[0]));

	for (unsigned int i = 1; i < typeItems.size(); i++) {
		std::vector<string> parameterItems;
		boost::split(parameterItems, typeItems[i], boost::is_any_of("="));

		if (parameterItems.size() != 2) {
			continue;
		}

		string key = boost::trim_copy(parameterItems[0]);
		string value = boost::trim_copy(parameterItems[1]);

		if (key == "charset") {
			_httpHeader.setContentCharset(Charset::detect(value));

		} else if (key == "boundary") {
			_httpHeader.setContentBoundary(value);
		}
	}
}


void Cgi::readQueryStringInputs()
{
	const char *inputsPtr = getenv("QUERY_STRING");
	if (inputsPtr == NULL) {
		return;
	}

	string inputs = inputsPtr;
	parse(inputs);
}

void Cgi::readContentInputs()
{
	HttpHeader::Method::Value method = _httpHeader.getMethod();
	if (method != HttpHeader::Method::POST && method != HttpHeader::Method::PUT) {
		return;
	}

	unsigned int size = readContentSize();
	if (size == 0) {
		return;
	}

	char inputsPtr[size + 1];
	memset(inputsPtr, 0, size + 1);

	std::cin.read(inputsPtr, size);
	if (std::cin.good() == false) {
		return;
	}

	string inputs = inputsPtr;

	MediaType::Value contentType = _httpHeader.getContentType();
	if (contentType == MediaType::APPLICATION_X_WWW_FORM_URL_ENCODED) {
		parse(inputs);

	} else if (contentType == MediaType::MULTIPART_FORM_DATA) {
		parseMultipart(inputs);
	}
}

unsigned int Cgi::readContentSize() const
{
	const char *sizePtr = getenv("CONTENT_LENGTH");
	if (sizePtr == NULL) {
		return 0;
	}

	unsigned int size = 0;
	try {
		size = boost::lexical_cast<unsigned int>(sizePtr);
	} catch (const  boost::bad_lexical_cast &e) {}

	return size;
}

void Cgi::readContentLanguages()
{
	const char *languagesPtr = getenv("CONTENT_LANGUAGE");
	if (languagesPtr == NULL) {
		return;
	}

	string languages = languagesPtr;

	std::vector<string> languagesList;
	boost::split(languagesList, languages, boost::is_any_of(","));

	for (auto languageStr: languagesList) {
		_httpHeader.addContentLanguage(Language::detect(languageStr));
	}
}

void Cgi::readAccepts()
{
	const char *acceptsPtr = getenv("HTTP_ACCEPT");
	if (acceptsPtr == NULL) {
		return;
	}

	string accepts = acceptsPtr;

	std::vector<string> acceptsList;
	boost::split(acceptsList, accepts, boost::is_any_of(","));

	for (auto accept: acceptsList) {
		std::vector<string> acceptItems;
		boost::split(acceptItems, accept, boost::is_any_of(";"));

		if (acceptItems.empty() == false) {
			_httpHeader.addAccept(MediaType::detect(acceptItems[0]));
		}
	}
}

void Cgi::readAcceptLanguages()
{
	const char *languagesPtr = getenv("HTTP_ACCEPT_LANGUAGE");
	if (languagesPtr == NULL) {
		return;
	}

	string languages = languagesPtr;

	std::vector<string> languagesList;
	std::multimap<double, Language::Value> languagesQuality;
	boost::split(languagesList, languages, boost::is_any_of(","));

	for (auto languageStr: languagesList) {
		std::vector<string> languageItems;
		boost::split(languageItems, languageStr, boost::is_any_of(";"));

		if (languageItems.empty()) {
			continue;
		}

		auto language = Language::detect(languageItems[0]);

		if (languageItems.size() != 2) {
			languagesQuality.insert(std::pair<double, Language::Value>(1, language));
			continue;
		}

		std::vector<string> qualityItems;
		boost::split(qualityItems, languageItems[1], boost::is_any_of("="));

		if (qualityItems.size() == 2) {
			double quality = boost::lexical_cast<double>(qualityItems[1]);
			languagesQuality.
				insert(std::pair<double, Language::Value>(quality, language));

		} else {
			languagesQuality.insert(std::pair<double, Language::Value>(1, language));
		}
	}

	for (auto languageQuality : boost::adaptors::reverse(languagesQuality)) {
		_httpHeader.addAcceptLanguage(languageQuality.second);
	}
}

void Cgi::readAcceptCharsets()
{
	const char *encodingsPtr = getenv("HTTP_ACCEPT_CHARSET");
	if (encodingsPtr == NULL) {
		return;
	}

	string encodings = encodingsPtr;

	std::vector<string> encondingsList;
	std::multimap<double, Charset::Value> encodingsQuality;
	boost::split(encondingsList, encodings, boost::is_any_of(","));

	for (auto encodingStr: encondingsList) {
		std::vector<string> encodingItems;
		boost::split(encodingItems, encodingStr, boost::is_any_of(";"));

		if (encodingItems.empty()) {
			continue;
		}

		auto encoding = Charset::detect(encodingItems[0]);

		if (encodingItems.size() != 2) {
			encodingsQuality.insert(std::pair<double, Charset::Value>(1, encoding));
			continue;
		}

		std::vector<string> qualityItems;
		boost::split(qualityItems, encodingItems[1], boost::is_any_of("="));

		if (qualityItems.size() == 2) {
			double quality = boost::lexical_cast<double>(qualityItems[1]);
			encodingsQuality.
				insert(std::pair<double, Charset::Value>(quality, encoding));

		} else {
			encodingsQuality.insert(std::pair<double, Charset::Value>(1, encoding));
		}
	}

	for (auto encodingQuality : boost::adaptors::reverse(encodingsQuality)) {
		_httpHeader.addAcceptCharset(encodingQuality.second);
	}
}

void Cgi::readCookies()
{
	const char *cookiesPtr = getenv("HTTP_COOKIE");
	if (cookiesPtr == NULL) {
		return;
	}

	string cookies = cookiesPtr;

	std::vector<string> keysValues;
	boost::split(keysValues, cookies, boost::is_any_of(";"));

	for (auto keyValue: keysValues) {
		boost::trim(keyValue);

		std::vector<string> keyValueSplitted;
		boost::split(keyValueSplitted, keyValue, boost::is_any_of("="));

		if (keyValueSplitted.size() == 2) {
			Cookie cookie;
			cookie.setKey(keyValueSplitted[0]);
			cookie.setValue(keyValueSplitted[1]);
			_httpHeader.addCookie(cookie);
		}
	}
}

void Cgi::readURI()
{
	_uri.clear();

	const char *scriptName = getenv("SCRIPT_NAME");
	if (scriptName != NULL) {
		_uri = scriptName;
	}

	const char *pathInfo = getenv("PATH_INFO");
	if (pathInfo != NULL) {
		_uri += pathInfo;
	}
}

void Cgi::readRemoteAddress()
{
	const char *remoteAddressPtr = getenv("REMOTE_ADDR");
	if (remoteAddressPtr == NULL) {
		return;
	}

	_remoteAddress = remoteAddressPtr;
}

void Cgi::parse(string inputs)
{
	decode(inputs);

	std::vector<string> keysValues;
	boost::split(keysValues, inputs, boost::is_any_of("&"));

	for (auto keyValue: keysValues) {
		std::vector<string> keyValueSplitted;
		boost::split(keyValueSplitted, keyValue, boost::is_any_of("="));
		if (keyValueSplitted.size() == 2) {
			_inputs[keyValueSplitted[0]] = keyValueSplitted[1];
		}
	}
}

void Cgi::parseMultipart(const string &inputs)
{
	string boundary = _httpHeader.getContentBoundary();
	if (boundary.empty()) {
		return;
	}

	UploadedFile uploadedFile;

	size_t position = 0;
	size_t firstOccurrence = 0;
	size_t secondOccurrence = 0;

	while (true) {
		firstOccurrence = inputs.find(boundary, position);

		if (firstOccurrence == string::npos) {
			break;
		}

		position += firstOccurrence + boundary.size();
		secondOccurrence = inputs.find(boundary, position);

		if (secondOccurrence == string::npos) {
			break;
		}

		unsigned int begin = firstOccurrence + boundary.size();
		unsigned int end = secondOccurrence - (firstOccurrence + boundary.size());

		uploadedFile.setMultipart(inputs.substr(begin, end));
		position = secondOccurrence + boundary.size();
	}

	if (uploadedFile.getControlName().empty() == false &&
	    uploadedFile.getFilename().empty() == false) {
		_files[uploadedFile.getControlName()] = uploadedFile.getFilename();
	}
}

void Cgi::decode(string &inputs)
{
	boost::trim(inputs);
	decodeSpecialSymbols(inputs);
	decodeHexadecimal(inputs);
	removeDangerousHtmlCharacters(inputs);
}

void Cgi::decodeSpecialSymbols(string &inputs)
{
	boost::replace_all(inputs, "+", " ");
}

void Cgi::decodeHexadecimal(string &inputs)
{
	boost::match_results<string::const_iterator> found;
	boost::regex hexadecimal("%[0-9A-F][0-9A-F]");

	while (boost::regex_search(inputs, found, hexadecimal)) {
		string finalHexadecimal = boost::to_upper_copy(found.str());
		string text = hexadecimalToText(finalHexadecimal);
		boost::replace_all(inputs, found.str(), text);
	}
}

string Cgi::hexadecimalToText(const string &hexadecimal)
{
	std::stringstream hexadecimalStream;
	hexadecimalStream << std::hex << hexadecimal.substr(1);

	unsigned int hexadecimalNumber = 0;
	hexadecimalStream >> hexadecimalNumber;

	string text("");
	text += char(hexadecimalNumber);
	return text;
}

void Cgi::removeDangerousHtmlCharacters(string &inputs)
{
	boost::replace_all(inputs, "'", "");
	boost::replace_all(inputs, "\"", "");
	boost::replace_all(inputs, "<", "");
	boost::replace_all(inputs, ">", "");
}

CGIPLUS_NS_END
