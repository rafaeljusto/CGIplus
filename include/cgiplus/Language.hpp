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

#ifndef __CGIPLUS_LANGUAGE_HPP__
#define __CGIPLUS_LANGUAGE_HPP__

#include <string>

#include "Cgiplus.hpp"

using std::string;

CGIPLUS_NS_BEGIN

/*! \class Language
 *  \brief Represents all supported languages.
 */
class Language
{
public:
	/*! List all types of language supported by Cgi.
	 */
	enum Value {
		ANY,
		ENGLISH_ANY,
		ENGLISH_GB,
		ENGLISH_US,
		PORTUGUESE_ANY,
		PORTUGUESE_BR,
		PORTUGUESE_PT,
		UNKNOWN
	};

	/*! Convert a language in string format into one of the value of
	 * Language::Value
	 *
	 * @param value Language in text format
	 * @return Enum item of the language
	 */
	static Value detect(const string &value);
};

CGIPLUS_NS_END

#endif // __CGIPLUS_LANGUAGE_HPP__
