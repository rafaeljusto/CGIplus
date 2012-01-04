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

#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <cgiplus/Builder.hpp>
#include <cgiplus/Cookie.hpp>

CGIPLUS_NS_BEGIN

string Builder::EOL("\r\n");

string Builder::Status::toString(const Value value, const string &message)
{
	if (value == UNDEFINED) {
		return string("");
	}

	return "Status: " + boost::lexical_cast<string>(value) + " " + message + EOL;
}

string Builder::Format::toString(const Value value)
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

	return valueToString + EOL;
}

Builder::Builder() :
	_form(""),
	_tags("<!-- ", " -->"),
	_status(Status::UNDEFINED, ""),
	_format(Format::HTML)
{
}

string& Builder::operator[](const string &key)
{
	return _fields[key];
}

Cookie& Builder::operator()(const string &key)
{
	auto cookieIt = _cookies.find(key);
	if (cookieIt != _cookies.end()) {
		return cookieIt->second;
	}

	Cookie cookie;
	cookie.setKey(key);
	_cookies[key] = cookie;

	return _cookies[key];
}

string Builder::build() const
{
	string header = Status::toString(_status.first, _status.second) +
		Format::toString(_format);

	for (auto cookie: _cookies) {
		header += cookie.second.build() + EOL;
	}

	header += EOL;

	string content = _form;
	for (auto field: _fields) {
		string key  = _tags.first + field.first + _tags.second;
		boost::replace_all(content, key, field.second);
	}

	return header + content;
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
	_form.clear();

	std::ifstream fileStream(formFile.c_str());
	if (fileStream.good()) {
		std::stringstream contentStream;
		contentStream << fileStream.rdbuf();
		_form = contentStream.str();
	}

	return *this;
}

Builder& Builder::setTags(const std::pair<string, string> &tags)
{
	_tags = tags;
	return *this;
}

Builder& Builder::setStatus(const Status::Value status, const string &message)
{
	_status = std::make_pair(status, message);
	return *this;
}

Builder& Builder::setFormat(const Format::Value format)
{
	_format = format;
	return *this;
}

Builder& Builder::clear()
{
	clearFields();
	clearCookies();
	return *this;
}

Builder& Builder::clearFields()
{
	_fields.clear();
	return *this;
}

Builder& Builder::clearCookies()
{
	_cookies.clear();
	return *this;
}

string Builder::redirect(const string &url)
{
	string header = "Location: " + url + EOL + EOL;
	return header;
}

CGIPLUS_NS_END
