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

#ifndef __CGIPLUS_MEDIATYPE_HPP__
#define __CGIPLUS_MEDIATYPE_HPP__

#include <set>
#include <string>

#include "Cgiplus.hpp"

using std::string;

CGIPLUS_NS_BEGIN

/*! \class MediaType
 *  \brief Store all possible media types
 */
class MediaType
{
public:
	/*! Media types
	 */
	enum Value {
		UNDEFINED,
		ANY,
		APPLICATION_ANY,
		APPLICATION_JSON,
		APPLICATION_XML,
		APPLICATION_X_WWW_FORM_URL_ENCODED,
		MULTIPART_ANY,
		MULTIPART_FORM_DATA,
		TEXT_ANY,
		TEXT_HTML,
		TEXT_PLAIN,
		TEXT_XML,
		UNKNOWN
	};

	/*! Convert a media type in string format into one of the value of
	 * MediaType::Value
	 *
	 * @param value Media type in text format
	 * @return Enum item of the media type
	 */
	static Value detect(const string &value);

	/*! Convert media type value into a string, in http header
	 * compliance (if requested)
	 *
	 * @param value MediaType::Value
	 * @param withLabel Add HTTP header label
	 * @return Media type in http header string representation
	 */
	static string toString(const Value value, const bool withLabel = false);

	/*! Convert media type values into a string, in http header
	 * compliance (if requested)
	 *
	 * @param values List of MediaType::Value
	 * @param withLabel Add HTTP header label
	 * @return Media types in http header string representation
	 */
	static string toString(const std::set<Value> values, 
	                       const bool withLabel = false);
};

CGIPLUS_NS_END

#endif // __CGIPLUS_MEDIATYPE_HPP__
