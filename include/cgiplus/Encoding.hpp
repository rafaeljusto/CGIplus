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

#ifndef __CGIPLUS_ENCODING_HPP__
#define __CGIPLUS_ENCODING_HPP__

#include <string>

#include "Cgiplus.hpp"

using std::string;

CGIPLUS_NS_BEGIN

/*! \class Encoding
 *  \brief Represents all supported encodings.
 */
class Encoding
{
public:
	/*! List all types of encodings supported by Cgi.
	 */
	enum Value {
		UNDEFINED,
		UTF8,
		UTF16,
		UTF32,
		ISO88591,
		UNKNOWN
	};

	/*! Convert a encoding in string format into one of the value of
	 * Encoding::Value
	 *
	 * @param value Encoding in text format
	 * @return Enum item of the encoding
	 */
	static Value detect(const string &value);

	/*! Convert enconding value into a string, in http header compliance
	 * (if requested)
	 *
	 * @param value Language::Value
	 * @param withLabel Add HTTP header label
	 * @return Language type in http header string representation
	 */
	static string toString(const Value value, const bool withLabel = false);
};

CGIPLUS_NS_END

#endif // __CGIPLUS_ENCODING_HPP__
