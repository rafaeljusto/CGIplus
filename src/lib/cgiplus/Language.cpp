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

string Language::toString(const Value value)
{
	string valueToString = "Content-Language: ";

	switch(value) {
	case ANY:
		valueToString += "*";
		break;
	case ENGLISH_ANY:
		valueToString += "en";
		break;
	case ENGLISH_GB:
		valueToString += "en-GB";
		break;
	case ENGLISH_US:
		valueToString += "en-US";
		break;
	case PORTUGUESE_ANY:
		valueToString += "pt";
		break;
	case PORTUGUESE_BR:
		valueToString += "pt-BR";
		break;
	case PORTUGUESE_PT:
		valueToString += "pt-PT";
		break;
	case UNKNOWN:
		break;
	}

	return valueToString;
}

bool Language::isEnglish(const std::set<Value> &values)
{
	for (Value value : values) {
		if (value == ANY || 
		    value == ENGLISH_ANY || 
		    value == ENGLISH_US || 
		    value == ENGLISH_GB) {
			return true;
		}
	}

	return false;
}

bool Language::isPortuguese(const std::set<Value> &values)
{
	for (Value value : values) {
		if (value == PORTUGUESE_ANY || 
		    value == PORTUGUESE_BR || 
		    value == PORTUGUESE_PT) {
			return true;
		}
	}

	return false;
}

CGIPLUS_NS_END
