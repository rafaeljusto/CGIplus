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

#ifndef __CGIPLUS_HTTP_HEADER_HPP__
#define __CGIPLUS_HTTP_HEADER_HPP__

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "Cgiplus.hpp"
#include "Cookie.hpp"
#include "Charset.hpp"
#include "Language.hpp"
#include "MediaType.hpp"

using std::string;

CGIPLUS_NS_BEGIN

/*! \class HttpHeader
 *  \brief Http header structure
 *
 * This class represents the HTTP header
 */
class HttpHeader
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
	HttpHeader();

	/*! Set http status. Possible values are defined in
	 * HttpHeader::Status::Value. By default is UNDEFINED.
	 *
	 * @param status Http status
	 * @param message Textual description of the error to be returned to
	 *                the client for human consumption
	 * @return Reference to the current object, allowing easy usability
	 */
	HttpHeader& setStatus(const Status::Value status, const string &message);

	/*! Return HTTP status.
	 *
	 * @return HTTP status
	 */
	std::pair<Status::Value, string> getStatus() const;

	/*! Set request method. Possible values are defined in
	 * HttpHeader::Method::Value. By default is UNDEFINED.
	 *
	 * @param method HTTP method
	 * @return Reference to the current object, allowing easy usability
	 */
	HttpHeader& setMethod(const Method::Value method);

	/*! Returns the request type related to REQUEST_METHOD enviroment
	 * variable. Possible types are defined in HttpHeader::Method::Value.
	 *
	 * @return Request type
	 */
	Method::Value getMethod() const;

	/*! Set HTTP location for redirection.
	 *
	 * @param location URL that you want to move to
	 * @return Reference to the current object, allowing easy usability
	 */
	HttpHeader& setLocation(const string &location);

	/*! Returns HTTP location.
	 *
	 * @return URL of the HTTP redirection
	 */
	string getLocation() const;

	/*! Set output format type. Possible values are defined in
	 * MediaType::Value. By default is UNDEFINED.
	 *
	 * @param type Output format type
	 * @return Reference to the current object, allowing easy usability
	 */
	HttpHeader& setContentType(const MediaType::Value type);

	/*!
	 */
	MediaType::Value getContentType() const;

	/*! Set output encoding type. Possible values are defined in
	 * Charset::Value. By default is UNDEFINED.
	 *
	 * @param charset Output format type
	 * @return Reference to the current object, allowing easy usability
	 */
	HttpHeader& setContentCharset(const Charset::Value charset);

	/*! Returns the encoding of the request.
	 *
	 * @return Request encoding
	 */
	Charset::Value getContentCharset() const;

	/*!
	 */
	HttpHeader& setContentBoundary(const string &boundary);

	/*!
	 */
	string getContentBoundary() const;

	/*! Add an output language. Possible values are defined in
	 * Language::Value.
	 *
	 * @param language Output language
	 * @return Reference to the current object, allowing easy usability
	 */
	HttpHeader& addContentLanguage(const Language::Value language);

	/*! Returns the request languages.
	 *
	 * @return List of languages of the request
	 */
	std::set<Language::Value> getContentLanguages() const;

	/*!
	 */
	HttpHeader& addAccept(const MediaType::Value accept);

	/*! Returns client supported response formats.
	 *
	 * @return List of media types that the client support
	 */
	std::set<MediaType::Value> getAccepts() const;

	/*!
	 */
	HttpHeader& addAcceptLanguage(const Language::Value language);

	/*! Returns client supported response languages.
	 *
	 * @return List of languages that the client support ordered by
	 * preference
	 */
	std::vector<Language::Value> getAcceptLanguages() const;

	/*!
	 */
	HttpHeader& addAcceptCharset(const Charset::Value charset);

	/*! Returns client supported response encodings.
	 *
	 * @return List of encodings that the client support ordered by
	 * preference
	 */
	std::vector<Charset::Value> getAcceptCharsets() const;

	/*!
	 */
	HttpHeader& addCookie(const Cookie &cookie);

	/*!
	 */
	HttpHeader& setCookies(const std::map<string, Cookie> &cookies);

	/*!
	 */
	std::map<string, Cookie> getCookies() const;

	/*!
	 */
	HttpHeader& clear();

	/*!
	 */
	string toString(const unsigned int contentSize) const;

	/*! Http header delimeter. It's public for tests purpouses.
	 */
	static string EOL;

private:
	std::pair<Status::Value, string> _status;
	Method::Value _method;
	string _location;

	// Content fields
	MediaType::Value _contentType;
	Charset::Value _contentCharset;
	string _contentBoundary;
	std::set<Language::Value> _contentLanguages;
	
	// Supported fields
	std::set<MediaType::Value> _accepts;
	std::vector<Language::Value> _acceptLanguages;
	std::vector<Charset::Value> _acceptCharsets;

	// Cookies
	std::map<string, Cookie> _cookies;
};

CGIPLUS_NS_END

#endif // __CGIPLUS_HTTP_HEADER_HPP__
