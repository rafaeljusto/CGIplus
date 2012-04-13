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

#include <boost/lexical_cast.hpp>

#include <cgiplus/Http.hpp>

CGIPLUS_NS_BEGIN

string Http::EOL("\r\n");

Http::Http() :
	_status(Status::UNDEFINED, ""),
	_method(Method::UNDEFINED),
	_type(MediaType::UNDEFINED),
	_encoding(Encoding::UNDEFINED),
	_boundary("")
{
}

void Http::setStatus(const std::pair<Status::Value, string> &status)
{
	_status = status;
}

std::pair<Http::Status::Value, string> Http::getStatus() const
{
	return _status;
}

void Http::setMethod(const Method::Value method)
{
	_method = method;
}

Http::Method::Value Http::getMethod() const
{
	return _method;
}

void Http::setType(const MediaType::Value type)
{
	_type = type;
}

MediaType::Value Http::getType() const
{
	return _type;
}

void Http::setEncoding(const Encoding::Value encoding)
{
	_encoding = encoding;
}

Encoding::Value Http::getEncoding() const
{
	return _encoding;
}

void Http::setBoundary(const string &boundary)
{
	_boundary = boundary;
}

string Http::getBoundary() const
{
	return _boundary;
}

void Http::addLanguage(const Language::Value language)
{
	_languages.insert(language);
}

std::set<Language::Value> Http::getLanguages() const
{
	return _languages;
}

void Http::addSupportedFormat(const MediaType::Value supportedFormat)
{
	_supportedFormats.insert(supportedFormat);
}

std::set<MediaType::Value> Http::getSupportedFormats() const
{
	return _supportedFormats;
}

void Http::addSupportedLanguage(const Language::Value supportedLanguage)
{
	_supportedLanguages.push_back(supportedLanguage);
}

std::vector<Language::Value> Http::getSupportedLanguages() const
{
	return _supportedLanguages;
}

void Http::addSupportedEncoding(const Encoding::Value supportedEncoding)
{
	_supportedEncodings.push_back(supportedEncoding);
}

std::vector<Encoding::Value> Http::getSupportedEncodings() const
{
	return _supportedEncodings;
}

void Http::addCookie(const std::pair<string, Cookie> &cookie)
{
	_cookies.insert(cookie);
}

std::map<string, Cookie> Http::getCookies() const
{
	return _cookies;
}

void Http::clear()
{
	_status = std::make_pair<Status::Value, string>(Status::UNDEFINED, "");
	_method = Method::UNDEFINED;
	_type = MediaType::UNDEFINED;
	_encoding = Encoding::UNDEFINED;
	_boundary.clear();
	_languages.clear();
	_supportedFormats.clear();
	_supportedLanguages.clear();
	_supportedEncodings.clear();
	_cookies.clear();
}

string Http::toString(const unsigned int contentSize) const
{
	string header("");

	if (_status.first != Status::UNDEFINED) {
		header += "Status: " + boost::lexical_cast<string>(_status.first) + " " + 
			_status.second + EOL;
	}

	if (_type != MediaType::UNDEFINED) {
		header += MediaType::toString(_type, true);

		if (_encoding != Encoding::UNDEFINED) {
			header += ";" + Encoding::toString(_encoding, true);
		}

		header += EOL;
	}
	
	header += "Content-Length: " + boost::lexical_cast<string>(contentSize) + EOL;

	if (_languages.empty() == false) {
		header += Language::toString(_languages, true) + EOL;
	}

	if (_supportedFormats.empty() == false) {
		header += MediaType::toString(_supportedFormats, true) + EOL;
	}

	if (_supportedLanguages.empty() == false) {
		header += Language::toString(_supportedLanguages, true) + EOL;
	}

	if (_supportedEncodings.empty() == false) {
		header += Encoding::toString(_supportedEncodings, true) + EOL;
	}

	for (auto cookie: _cookies) {
		header += cookie.second.build() + EOL;
	}

	header += EOL;

	return header;
}

CGIPLUS_NS_END
