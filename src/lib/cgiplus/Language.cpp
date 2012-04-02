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

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <cgiplus/Language.hpp>

CGIPLUS_NS_BEGIN

Language::Value Language::detect(const string &value)
{
	string language = boost::algorithm::to_lower_copy(value);
	boost::trim(language);

	if (language == "*" || language == "*-*") {
		return ANY;

	} else if (language == "en" || language == "en-*") {
		return ENGLISH_ANY;

	} else if (language == "en-gb") {
		return ENGLISH_GB;

	} else if (language == "en-us") {
		return ENGLISH_US;
	
	} else if (language == "pt" || language == "pt-*") {
		return PORTUGUESE_ANY;
	
	} else if (language == "pt-br") {
		return PORTUGUESE_BR;

	} else if (language == "pt-pt") {
		return PORTUGUESE_PT;
	
	} else {
		return UNKNOWN;
	}
}

CGIPLUS_NS_END
