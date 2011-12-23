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

#include <boost/lexical_cast.hpp>

#include "Cgiplus.hpp"

using std::string;

CGIPLUS_NS_BEGIN

/*! \class Cgi
 *  \brief Parse HTTP server requests.
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
			CONNECT,
			DELETE,
			HEAD,
			GET,
			OPTIONS,
			PATCH,
			POST,
			PUT,
			TRACE
		};
	};

	/*! \class Source
	 *  \brief Represents the different data types stored in Cgi.
	 */
	class Source
	{
	public:
		/*! List all types of stored in Cgi.
		 */
		enum Value {
			FIELD,
			COOKIE,
			FILE
		};
	};

	/*! The HTTP server enviroment variables are parsed in the
	 * constructor.
	 *
	 * @see readInputs
	 */
	Cgi();

	/*! All uploaded files are removed in destructor
	 */
	~Cgi();

	/*! Access request fields retrieved from QUERY_STRING enviroment
	 * variable.
	 *
	 * @param key Field key
	 * @return Field value
	 */
	string operator[](const string &key) const;

	/*! Access request cookies retieved from HTTP_COOKIE enviroment
	 * variale.
	 *
	 * @param key Cookie key
	 * @return Cookie value
	 */
	string operator()(const string &key) const;

	/*! Access all data types retrieved by the CGI. You can also convert
	 * the data using a callback function.
	 *
	 * @tparam T Type of the data that is going to be returned.
	 * @param key Data key
	 * @param source Possible data sources (Check Cgi::Source for
	 *               possible values).
	 * @param converter callback function to convert the data value into
	 *                  the type that you want.
	 * @return Data value (in the desired format, by default is string),
	 *         when the data is not found an empty type is going to be
	 *         returned.
	 */
	template<class T = string>
	T get(const string &key, 
	      const Source::Value source = Source::FIELD,
	      T (*converter)(const string&) = boost::lexical_cast<T>) const
	{
		T value;

		if (source == Source::FIELD) {
			auto input = _inputs.find(key);
			if (input != _inputs.end()) {
				value = converter(input->second);
			}
		
		} else if (source == Source::COOKIE) {
			auto cookie = _cookies.find(key);
			if (cookie != _cookies.end()) {
				value = converter(cookie->second);
			}
		
		} else if (source == Source::FILE) {
			auto file = _files.find(key);
			if (file != _files.end()) {
				value = converter(file->second);
			}
		}

		return value;
	}

	/*! Allow converting all source data into a desired object using a
	 * callback function.
	 *
	 * @tparam T Type of the data that is going to be returned.
	 * @param source Possible data sources (Check Cgi::Source for
	 *               possible values).
	 * @param converter callback function to convert the data value into
	 *                  the type that you want.
	 * @return Data value (in the desired format).
	 */
	template<class T>
	T get(const Source::Value source, 
	      T (*converter)(std::map<string, string>)) const
	{
		switch(source) {
		case Source::FIELD:
			return converter(_inputs);
		case Source::COOKIE:
			return converter(_cookies);
		case Source::FILE:
			return converter(_files);			
		};

		return T();
	}

	/*! Parse Apche envirment variables. The constructor already calls
	 * this method, so you don't need to call it again unless you have
	 * some special case.
	 *
	 * The enviroment variables current parsed are REQUEST_METHOD,
	 * CONTENT_LENGTH, CONTENT_TYPE, QUERY_STRING, HTTP_COOKIE,
	 * REMOTE_ADDR.
	 *
	 * @todo Upload binary files. For now is only allowed text/plain.
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
	string parseBoundary(const string &type);
	void parseMultipart(const string &inputs, const string &boundary);

	void decode(string &inputs);
	void decodeSpecialSymbols(string &inputs);
	void decodeHexadecimal(string &inputs);
	string hexadecimalToText(const string &hexadecimal);
	void removeDangerousHtmlCharacters(string &inputs);

	Method::Value _method;
	std::map<string, string> _inputs;
	std::map<string, string> _cookies;
	std::map<string, string> _files;
	string _remoteAddress;
};

CGIPLUS_NS_END

#endif // __CGIPLUS_CGI_H__
