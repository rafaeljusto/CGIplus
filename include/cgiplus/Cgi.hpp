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

#ifndef __CGIPLUS_CGI_H__
#define __CGIPLUS_CGI_H__

#include <map>
#include <string>

#include "Cgiplus.hpp"

using std::string;

CGIPLUS_NS_BEGIN

/*! \class Cgi
 *  \brief Parse Apache requests.
 *
 * Use this class to get all data sent by the client browser.
 */
class Cgi
{
public:
	/*! \class Method
	 *  \brief Represents a request method.
	 */
	class Method
	{
	public:
		/*! List all types of request methods.
		 */
		enum Value {
			UNKNOWN,
			GET,
			POST
		};
	};

	/*! The Apache enviroment variables are parsed in the constructor.
	 *
	 * @see readInputs
	 */
	Cgi();

	/*! Access request fields retrieved from QUERY_STRING enviroment
	 * variable.
	 *
	 * @param key Field key
	 * @return Field value
	 */
	string operator[](const string &key);

	/*! Access request cookies retieved from HTTP_COOKIE enviroment
	 * variale.
	 *
	 * @param key Cookie key
	 * @return Cookie value
	 */
	string operator()(const string &key);

	/*! Parse Apche envirment variables. The constructor already calls
	 * this method, so you don't need to call it again unless you have
	 * some special case.
	 *
	 * The enviroment variables current parsed are REQUEST_METHOD,
	 * CONTENT_LENGTH, CONTENT_TYPE, QUERY_STRING, HTTP_COOKIE,
	 * REMOTE_ADDR.
	 *
	 * @todo Support multipart/form-data
	 */
	void readInputs();

	/*! Returns the request type related to REQUEST_METHOD enviroment
	 * variable. Possible types are defined in Cgi::Method::Value.
	 *
	 * @return Request type
	 */
	Method::Value getMethod() const;

	/*! Returns the number of fields parsed. Usefull for testing.
	 *
	 * @return Number of fields parsed
	 */
	unsigned int getNumberOfInputs() const;

	/*! Returns the number of cookies parsed. Usefull for testing.
	 *
	 * @return Number of cookies parsed
	 */
	unsigned int getNumberOfCookies() const;

	/*! Returns client IP address retrieved from REMOTE_ADDR enviroment
	 * variable.
	 *
	 * @return Client IP address
	 */
	string getRemoteAddress() const;

private:
	void clearInputs();
	void readMethod();
	void readGetInputs();
	void readPostInputs();
	void readCookies();
	void readRemoteAddress();

	void parse(string inputs);

	void decode(string &inputs);
	void decodeSpecialSymbols(string &inputs);
	void decodeHexadecimal(string &inputs);
	string hexadecimalToText(const string &hexadecimal);
	void removeDangerousHtmlCharacters(string &inputs);

	Method::Value _method;
	std::map<string, string> _inputs;
	std::map<string, string> _cookies;
	string _remoteAddress;
};

CGIPLUS_NS_END

#endif // __CGIPLUS_CGI_H__
