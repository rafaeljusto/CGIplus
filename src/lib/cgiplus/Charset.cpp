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

#include <cgiplus/Charset.hpp>

CGIPLUS_NS_BEGIN

Charset::Value Charset::detect(const string &value)
{
	string charset = boost::algorithm::to_lower_copy(value);
	boost::trim(charset);

	if (charset == "utf-8") {
		return UTF8;
	
	} else if (charset == "utf-16") {
		return UTF16;
	
	} else if (charset == "utf-32") {
		return UTF32;
	
	} else if (charset == "iso-8859-1") {
		return ISO88591;
	
	} else {
		return UNKNOWN;
	}
}

string Charset::toString(const Value value)
{
	switch(value) {
	case UNDEFINED:
		break;
	case UTF8:
		return "utf-8";
	case UTF16:
		return "utf-16";
	case UTF32:
		return "utf-32";
	case ISO88591:
		return "iso-8859-1";
	case UNKNOWN:
		break;
	}

	return "";
}

CGIPLUS_NS_END
