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

#ifndef __CGIPLUS_CGI_H__
#define __CGIPLUS_CGI_H__

#include <map>
#include <string>

#include <boost/lexical_cast.hpp>

#include "Cgiplus.hpp"

using std::string;

CGIPLUS_NS_BEGIN

class Cgi
{
public:
	class Method
	{
	public:
		enum Value {
			UNKNOWN,
			GET,
			POST
		};
	};

	Cgi();

	string operator[](const string &key);
	string operator()(const string &key);

	void readInputs();

	Method::Value getMethod() const;
	unsigned int getNumberOfInputs() const;
	unsigned int getNumberOfCookies() const;

private:
	void clearInputs();
	void readMethod();
	void readGetInputs();
	void readPostInputs();
	void readCookies();

	void parse(string inputs);

	void decode(string &inputs);
	void decodeSpecialSymbols(string &inputs);
	void decodeHexadecimal(string &inputs);
	string hexadecimalToText(const string &hexadecimal);
	void removeDangerousHtmlCharacters(string &inputs);

	Method::Value _method;
	std::map<string, string> _inputs;
	std::map<string, string> _cookies;
};

CGIPLUS_NS_END

#endif // __CGIPLUS_CGI_H__
