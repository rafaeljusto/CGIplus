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

#ifndef __CGI_PLUS_BUILDER_H__
#define __CGI_PLUS_BUILDER_H__

#include <map>
#include <string>

#include "Cgiplus.hpp"
#include "Cookie.hpp"

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
	/*! \class Format
	 *  \brief Represents a html output format type.
	 */
	class Format
	{
	public:
		/*! List all types of format.
		 */
		enum Value {
			PLAIN_TEXT,
			HTML
		};

		/*! Convert format into html header compliance text.
		 *
		 * @param value Output format type
		 * @return Text to be added into html header
		 */
		static string toString(const Value value);
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

	/*! Set html output format type. Possible values are defined in
	 * Builder::Format::Value. By default is HTML.
	 *
	 * @param format Html output format type.
	 * @return Reference to the current object, allowing easy usability
	 */
	Builder& setFormat(const Format::Value format);

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
	Format::Value _format;
};

CGIPLUS_NS_END

#endif // __CGI_PLUS_BUILDER_H__
