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

#include <cgiplus/Encoding.hpp>

CGIPLUS_NS_BEGIN

Encoding::Value Encoding::detect(const string &value)
{
	string encoding = boost::algorithm::to_lower_copy(value);
	boost::trim(encoding);

	if (encoding == "utf-8") {
		return UTF8;
	
	} else if (encoding == "utf-16") {
		return UTF16;
	
	} else if (encoding == "utf-32") {
		return UTF32;
	
	} else if (encoding == "iso-8859-1") {
		return ISO88591;
	
	} else {
		return UNKNOWN;
	}
}

string Encoding::toString(const Value value, const bool withLabel)
{
	string valueToString("");
	
	if (withLabel) {
		valueToString = "charset=";
	}

	switch(value) {
	case UNDEFINED:
	case ANY:
		break;
	case UTF8:
		valueToString += "utf-8";
		break;
	case UTF16:
		valueToString += "utf-16";
		break;
	case UTF32:
		valueToString += "utf-32";
		break;
	case ISO88591:
		valueToString += "iso-8859-1";
		break;
	case UNKNOWN:
		break;
	}

	return valueToString;
}

string Encoding::toString(const std::vector<Value> values, const bool withLabel)
{
	string valueToString("");

	if (withLabel) {
		valueToString = "Accept-Charset: ";
	}

	for (Value value : values) {
		valueToString += toString(value) + ",";
	}

	if (values.empty() == false) {
		valueToString = valueToString.substr(0, valueToString.size() - 1);
	}

	return valueToString;
}

CGIPLUS_NS_END
