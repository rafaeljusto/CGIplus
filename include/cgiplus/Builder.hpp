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

#ifndef __CGIPLUS_BUILDER_H__
#define __CGIPLUS_BUILDER_H__

#include <map>
#include <string>
#include <utility>

#include "Cgiplus.hpp"
#include "Cookie.hpp"
#include "HttpHeader.hpp"

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
	/*! Nothing special here, just initializing everything.
	 */
	Builder();

	/*! Sets a field to replace a tag in template.
	 *
	 * @param key Key that represents a template tag
	 * @return Value by reference (same way as std::map does)
	 */
	string& operator[](const string &key);

	/*! Add content to form that will be print in output
	 *
	 * @param content Form content
	 * @return The current builder object
	 */
	Builder& operator<<(const string &content);

	/*! Set HTTP header fields
	 *
	 * @return HTTP header object
	 */
	HttpHeader* operator->();

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
	 * @param content Template content or real content
	 * @return Reference to the current object, allowing easy usability
	 */
	Builder& setContent(const string &content);

	/*! Sets template file path. If there's any error opening the file,
	 * the template content is going to be empty.
	 *
	 * @param templateFile Template file path
	 * @return Reference to the current object, allowing easy usability
	 */
	Builder& setTemplateFile(const string &templateFile);

	/*! Set template's tag delimeter. By default is used <!-- and -->.
	 * @param tags Defines the beggining and the end of the tag delimeter
	 * @return Reference to the current object, allowing easy usability
	 */
	Builder& setTags(const std::pair<string, string> &tags);

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

private:
	HttpHeader _httpHeader;
	string _content;
	std::pair<string, string> _tags;
	std::map<string, string> _fields;
};

CGIPLUS_NS_END

#endif // __CGIPLUS_BUILDER_H__
