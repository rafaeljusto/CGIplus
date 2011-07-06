#include <cstdlib>
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

CGIPLUS_NS_BEGIN

Cgi::Cgi() :
	_method(Method::UNKNOWN)
{
	readInputs();
}

string Cgi::operator[](const string &key)
{
	auto input = _inputs.find(key);
	if (input != _inputs.end()) {
		return input->second;
	}

	return "";
}

string Cgi::operator()(const string &key)
{
	auto cookie = _cookies.find(key);
	if (cookie != _cookies.end()) {
		return cookie->second;
	}

	return "";
}

void Cgi::readInputs()
{
	readMethod();
	readGetInputs();
	readPostInputs();
	readCookies();
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

	string type = typePtr;
	if (type != "application/x-www-form-urlencoded") {
		// http://www.w3.org/TR/html401/interact/forms.html#h-17.13.4
		// TODO: Upload file -> multipart/form-data
		return;
	}

	int size = 0;
	try {
		size = boost::lexical_cast<int>(sizePtr);
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

	string inputs = inputsPtr;
	parse(inputs);
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
