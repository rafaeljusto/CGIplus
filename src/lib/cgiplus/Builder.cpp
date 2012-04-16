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

Builder::Builder() :
	_content(""),
	_tags("<!-- ", " -->")
{
}

string& Builder::operator[](const string &key)
{
	return _fields[key];
}

Builder& Builder::operator<<(const string &content)
{
	_content += content;
	return *this;
}

HttpHeader* Builder::operator->()
{
	return &_httpHeader;
}

string Builder::build() const
{
	string content = _content;
	for (auto field: _fields) {
		string key  = _tags.first + field.first + _tags.second;
		boost::replace_all(content, key, field.second);
	}

	return _httpHeader.toString(content.size()) + content;
}

void Builder::show() const
{
	std::cout << build() << std::endl;
}

Builder& Builder::setContent(const string &content)
{
	_content = content;
	return *this;
}

Builder& Builder::setTemplateFile(const string &templateFile)
{
	_content.clear();

	std::ifstream fileStream(templateFile.c_str());
	if (fileStream.good()) {
		std::stringstream contentStream;
		contentStream << fileStream.rdbuf();
		_content = contentStream.str();
	}

	return *this;
}

Builder& Builder::setTags(const std::pair<string, string> &tags)
{
	_tags = tags;
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
	_httpHeader.getCookies().clear();
	return *this;
}

CGIPLUS_NS_END
