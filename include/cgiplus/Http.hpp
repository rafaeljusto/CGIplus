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

#ifndef __CGIPLUS_HTTP_HPP__
#define __CGIPLUS_HTTP_HPP__

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "Cgiplus.hpp"
#include "Cookie.hpp"
#include "Encoding.hpp"
#include "Language.hpp"
#include "MediaType.hpp"

using std::string;

CGIPLUS_NS_BEGIN

/*! \class Http
 *  \brief Http header structure
 *
 * This class represents the HTTP header
 */
class Http
{
public:
	/*! \class Status
	 *  \brief Represents a HTTP response status
	 */
	class Status
	{
	public:
		/*! List all HTTP response status
		 */
		enum Value {
			UNDEFINED = 0,

			// 1xx: Informational - Request received, continuing process
			CONTINUE = 100,
			SWITCHING_PROTOCOLS,

			// 2xx: Success - The action was successfully received,
			// understood, and accepted
			OK = 200,
			CREATED,
			ACCEPTED,
			NON_AUTHORITATIVE_INFORMATION,
			NO_CONTENT,
			RESET_CONTENT,
			PARTIAL_CONTENT,

			// 3xx: Redirection - Further action must be taken in order to
			// complete the request
			MULTIPLE_CHOICES = 300,
			MOVED_PERMANENTLY,
			FOUND,
			SEE_OTHER,
			NOT_MODIFIED,
			USE_PROXY,
			TEMPORARY_REDIRECT,

			// 4xx: Client Error - The request contains bad syntax or cannot
			// be fulfilled
			BAD_REQUEST = 400,
			UNAUTHORIZED,
			PAYMENT_REQUIRED,
			FORBIDDEN,
			NOT_FOUND,
			METHOD_NOT_ALLOWED,
			NOT_ACCEPTABLE,
			PROXY_AUTHENTICATION_REQUIRED,
			REQUEST_TIME_OUT,
			CONFLICT,
			GONE,
			LENGTH_REQUIRED,
			PRECONDITION_FAILED,
			REQUEST_ENTITY_TOO_LARGE,
			REQUEST_URI_TOO_LARGE,
			UNSUPPORTED_MEDIA_TYPE,
			REQUESTED_RANGE_NOT_SATISFIABLE,
			EXPECTATION_FAILED,

			// 5xx: Server Error - The server failed to fulfill an
			// apparently valid request
			INTERNAL_SERVER_ERROR = 500,
			BAD_GATEWAY,
			SERVICE_UNAVAILABLE,
			GATEWAY_TIME_OUT,
			HTTP_VERSION_NOT_SUPPORTED
		};
	};

	/*! \class Method
	 *  \brief Represents a request method.
	 */
	class Method
	{
	public:
		/*! List all types of request methods.
		 */
		enum Value {
			UNDEFINED,
			CONNECT,
			DELETE,
			HEAD,
			GET,
			OPTIONS,
			PATCH,
			POST,
			PUT,
			TRACE,
			UNKNOWN
		};
	};

	/*! Default constructor
	 */
	Http();

	void setStatus(const std::pair<Status::Value, string> &status);
	std::pair<Status::Value, string> getStatus() const;

	void setMethod(const Method::Value method);
	Method::Value getMethod() const;

	void setType(const MediaType::Value type);
	MediaType::Value getType() const;

	void setEncoding(const Encoding::Value encoding);
	Encoding::Value getEncoding() const;

	void setBoundary(const string &boundary);
	string getBoundary() const;

	void addLanguage(const Language::Value language);
	std::set<Language::Value> getLanguages() const;

	void addSupportedFormat(const MediaType::Value supportedFormat);
	std::set<MediaType::Value> getSupportedFormats() const;

	void addSupportedLanguage(const Language::Value supportedLanguage);
	std::vector<Language::Value> getSupportedLanguages() const;

	void addSupportedEncoding(const Encoding::Value supportedEncoding);
	std::vector<Encoding::Value> getSupportedEncodings() const;

	void addCookie(const std::pair<string, Cookie> &cookie);
	std::map<string, Cookie> getCookies() const;

	void clear();
	string toString(const unsigned int contentSize) const;

	/*! Http header delimeter. It's public for tests purpouses.
	 */
	static string EOL;

private:
	std::pair<Status::Value, string> _status;
	Method::Value _method;
	MediaType::Value _type;
	Encoding::Value _encoding;
	string _boundary;
	std::set<Language::Value> _languages;
	std::set<MediaType::Value> _supportedFormats;
	std::vector<Language::Value> _supportedLanguages;
	std::vector<Encoding::Value> _supportedEncodings;
	std::map<string, Cookie> _cookies;
};

CGIPLUS_NS_END

#endif // __CGIPLUS_HTTP_HPP__
