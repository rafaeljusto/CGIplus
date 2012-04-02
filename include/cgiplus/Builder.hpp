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

#ifndef __CGI_PLUS_BUILDER_H__
#define __CGI_PLUS_BUILDER_H__

#include <map>
#include <string>
#include <utility>

#include "Cgiplus.hpp"
#include "Cookie.hpp"
#include "MediaType.hpp"

using std::string;

CGIPLUS_NS_BEGIN

/*! \class Builder
 *  \brief Build html outputs using templates.
 *
 * Use this class to create your html output using templates and to
 * set your cookies. This class automatically creates the html
 * headers.
 */
class Builder
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

		/*! Convert http status into html header compliance text.
		 *
		 * @param value Http status
		 * @param message Textual description of the error to be returned to
		 *                the client for human consumption
		 * @return Text to be added into http header
		 */
		static string toString(const Value value, const string &message);
	};

	/*! Nothing special here, just initializing everything.
	 */
	Builder();

	/*! Sets a field to replace a tag in template.
	 *
	 * @param key Key that represents a template tag
	 * @return Value by reference (same way as std::map does)
	 */
	string& operator[](const string &key);

	/*! Sets a cookie to be defined in client's browser.
	 *
	 * @param key Key that will represent the cookie
	 * @return Cookie object by reference (same way as std::map does)
	 */
	Cookie& operator()(const string &key);

	/*! Parse the given template replacing all tags with the given keys/values.
	 *
	 * @return Parsed template file already with html header
	 */
	string build() const;

	/*! Same as build method, but it already print the content into
	 * standart output.
	 */
	void show() const;

	/*! Sets template content. By default is empty.
	 *
	 * @param form Template content
	 * @return Reference to the current object, allowing easy usability
	 */
	Builder& setForm(const string &form);

	/*! Sets template file path. If there's any error opening the file,
	 * the template content is going to be empty.
	 *
	 * @param formFile Template file path
	 * @return Reference to the current object, allowing easy usability
	 */
	Builder& setFormFile(const string &formFile);

	/*! Set template's tag delimeter. By default is used <!-- and -->.
	 * @param tags Defines the beggining and the end of the tag delimeter
	 * @return Reference to the current object, allowing easy usability
	 */
	Builder& setTags(const std::pair<string, string> &tags);

	/*! Set http status. Possible values are defined in
	 * Builder::Status::Value. By default is UNDEFINED.
	 *
	 * @param status Http status
	 * @param message Textual description of the error to be returned to
	 *                the client for human consumption
	 * @return Reference to the current object, allowing easy usability
	 */
	Builder& setStatus(const Status::Value status, const string &message);

	/*! Set output format type. Possible values are defined in
	 * MediaType::Value. By default is HTML.
	 *
	 * @param format Output format type
	 * @return Reference to the current object, allowing easy usability
	 */
	Builder& setFormat(const MediaType::Value format);

	/*! Remove all fields and cookies from builder.
	 *
	 * @return Reference to the current object, allowing easy usability
	 */
	Builder& clear();

	/*! Remove all fields from builder.
	 *
	 * @return Reference to the current object, allowing easy usability
	 */
	Builder& clearFields();

	/*! Remove all cookies from builder.
	 *
	 * @return Reference to the current object, allowing easy usability
	 */
	Builder& clearCookies();

	/*! Returns html header text necessary to redirect the webpage to
	 * another url.
	 *
	 * @param url Url that you want to be redirect to
	 * @return Html header text
	 */
	static string redirect(const string &url);

	/*! Html header delimeter. It's public for tests purpouses.
	 */
	static string EOL;

private:
	string _form;
	std::pair<string, string> _tags;
	std::map<string, string> _fields;
	std::map<string, Cookie> _cookies;
	std::pair<Status::Value, string> _status;
	MediaType::Value _format;
};

CGIPLUS_NS_END

#endif // __CGI_PLUS_BUILDER_H__
