#include <iostream>
#include <fstream>

#include <boost/algorithm/string/replace.hpp>

#include <cgiplus/Builder.hpp>

CGIPLUS_NS_BEGIN

string Builder::Method::toString(const Value value)
{
	string valueToString = "Content-type: ";

	switch (value) {
	case PLAIN_TEXT:
		valueToString += "text/plain";
		break;
	case HTML:
		valueToString += "text/html";
		break;
	}

	valueToString += "\n\r\n\r";
	return valueToString;
}

Builder::Builder() :
	_form(""),
	_tags("<!-- ", " -->"),
	_method(Builder::Method::HTML)
{
}

string& Builder::operator[](const string &key)
{
	return _fields[key];
}

Cookie& Builder::operator()(const string &key)
{
	auto cookieIt = _cookies.find(key);
	if (cookieIt == _cookies.end()) {
		return cookieIt->second;
	}

	Cookie cookie;
	cookie.setKey(key);
	_cookies[key] = cookie;

	return _cookies[key];
}

string Builder::build() const
{
	string content = _form;
	for (auto field: _fields) {
		string key  = _tags.first + field.first + _tags.second;
		boost::replace_all(content, key, field.second);
	}
	return Method::toString(_method) + content;
}

void Builder::show() const
{
	std::cout << build() << std::endl;
}

Builder& Builder::setForm(const string &form)
{
	_form = form;
	return *this;
}

Builder& Builder::setFormFile(const string &formFile)
{
	std::ifstream formFileStream(formFile.c_str());
	while (formFileStream.good()) {
		string line;
		std::getline(formFileStream, line);
		_form += line;
	}
	return *this;
}

Builder& Builder::setTags(const std::pair<string, string> &tags)
{
	_tags = tags;
	return *this;
}

Builder& Builder::setMethod(const Builder::Method::Value method)
{
	_method = method;
	return *this;
}

CGIPLUS_NS_END
