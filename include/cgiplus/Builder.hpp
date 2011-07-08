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

#ifndef __CGI_PLUS_BUILDER_H__
#define __CGI_PLUS_BUILDER_H__

#include <map>
#include <string>

#include "Cgiplus.hpp"
#include "Cookie.hpp"

using std::string;

CGIPLUS_NS_BEGIN

class Builder
{
public:
	class Method
	{
	public:
		enum Value {
			PLAIN_TEXT,
			HTML
		};

		static string toString(const Value value);
	};

	Builder();

	string& operator[](const string &key);
	Cookie& operator()(const string &key);

	string build() const;
	void show() const;

	Builder& setForm(const string &form);
	Builder& setFormFile(const string &formFile);
	Builder& setTags(const std::pair<string, string> &tags);
	Builder& setMethod(const Method::Value method);

private:
	string _form;
	std::pair<string, string> _tags;
	Method::Value _method;
	std::map<string, string> _fields;
	std::map<string, Cookie> _cookies;
};

CGIPLUS_NS_END

#endif // __CGI_PLUS_BUILDER_H__
