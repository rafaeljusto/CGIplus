/*
  CGIplus Copyright (C) 2011 Rafael Dantas Justo

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
#include <sstream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include <cgiplus/Cgi.hpp>
#include <cgiplus/UploadedFile.hpp>

CGIPLUS_NS_BEGIN

Cgi::Cgi() :
	_method(Method::UNKNOWN),
	_remoteAddress("")
{
	readInputs();
}

Cgi::~Cgi()
{
	for(auto file: _files) {
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

void Cgi::readInputs()
{
	clearInputs();
	readMethod();
	readGetInputs();
	readPostInputs();
	readCookies();
	readRemoteAddress();
}

Cgi::Method::Value Cgi::getMethod() const
{
	return _method;
}

unsigned int Cgi::getNumberOfInputs() const
{
	return _inputs.size();
}

unsigned int Cgi::getNumberOfCookies() const
{
	return _cookies.size();
}

string Cgi::getRemoteAddress() const
{
	return _remoteAddress;
}

void Cgi::clearInputs()
{
	_method = Method::UNKNOWN;
	_inputs.clear();
	_cookies.clear();
	_files.clear();
	_remoteAddress.clear();
}

void Cgi::readMethod()
{
	const char *methodPtr = getenv("REQUEST_METHOD");
	if (methodPtr != NULL) {
		string method = boost::to_upper_copy((string) methodPtr);
		if (method == "GET") {
			_method = Method::GET;
		} else if (method == "POST") {
			_method = Method::POST;
		}
	}
}

void Cgi::readGetInputs()
{
	const char *inputsPtr = getenv("QUERY_STRING");
	if (inputsPtr == NULL) {
		return;
	}

	string inputs = inputsPtr;
	parse(inputs);
}

void Cgi::readPostInputs()
{
	const char *sizePtr = getenv("CONTENT_LENGTH");
	const char *typePtr = getenv("CONTENT_TYPE");

	if (_method != Method::POST || sizePtr == NULL || typePtr == NULL) {
		return;
	}

	unsigned int size = 0;
	try {
		size = boost::lexical_cast<unsigned int>(sizePtr);
	} catch (const  boost::bad_lexical_cast &e) {}

	if (size == 0) {
		return;
	}

	char inputsPtr[size + 1];
	memset(inputsPtr, 0, size + 1);

	std::cin.read(inputsPtr, size);
	if (std::cin.good() == false) {
		return;
	}

	string type = typePtr;
	string inputs = inputsPtr;

	if (type == "application/x-www-form-urlencoded") {
		parse(inputs);

	} else if (type.find("multipart/form-data") != string::npos) {
		string boundary = parseBoundary(type);
		parseMultipart(inputs, boundary);
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
	boost::split(keysValues, cookies, boost::is_any_of("; "));

	for (string &keyValue: keysValues) {
		std::vector<string> keyValueSplitted;
		boost::split(keyValueSplitted, keyValue, boost::is_any_of("="));
		if (keyValueSplitted.size() == 2) {
			_cookies[keyValueSplitted[0]] = keyValueSplitted[1];
		}
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

string Cgi::parseBoundary(const string &type)
{
	std::vector<string> keyValues;
	boost::split(keyValues, type, boost::is_any_of(";"));

	if (keyValues.size() != 2) {
		return "";
	}

	std::vector<string> keyValueSplitted;
	boost::split(keyValueSplitted, keyValues[1], boost::is_any_of("="));

	if (keyValueSplitted.size() != 2) {
		return "";
	}

	return keyValueSplitted[1];
}

void Cgi::parseMultipart(const string &inputs, const string &boundary)
{
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
