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

#include <cgiplus/HttpHeader.hpp>

CGIPLUS_NS_BEGIN

string HttpHeader::EOL("\r\n");

HttpHeader::HttpHeader() :
	_status(Status::UNDEFINED, ""),
	_method(Method::UNDEFINED),
	_location(""),
	_contentType(MediaType::UNDEFINED),
	_contentCharset(Charset::UNDEFINED),
	_contentBoundary("")
{
}

HttpHeader& 
HttpHeader::setStatus(const Status::Value status, const string &message)
{
	_status = std::make_pair(status, message);
	return *this;
}

std::pair<HttpHeader::Status::Value, string> HttpHeader::getStatus() const
{
	return _status;
}

HttpHeader& HttpHeader::setMethod(const Method::Value method)
{
	_method = method;
	return *this;
}

HttpHeader::Method::Value HttpHeader::getMethod() const
{
	return _method;
}

HttpHeader& HttpHeader::setLocation(const string &location)
{
	_location = location;
	return *this;
}

string HttpHeader::getLocation() const
{
	return _location;
}

HttpHeader& HttpHeader::setContentType(const MediaType::Value type)
{
	_contentType = type;
	return *this;
}

MediaType::Value HttpHeader::getContentType() const
{
	return _contentType;
}

HttpHeader& HttpHeader::setContentCharset(const Charset::Value charset)
{
	_contentCharset = charset;
	return *this;
}

Charset::Value HttpHeader::getContentCharset() const
{
	return _contentCharset;
}

HttpHeader& HttpHeader::setContentBoundary(const string &boundary)
{
	_contentBoundary = boundary;
	return *this;
}

string HttpHeader::getContentBoundary() const
{
	return _contentBoundary;
}

HttpHeader& HttpHeader::addContentLanguage(const Language::Value language)
{
	_contentLanguages.insert(language);
	return *this;
}

std::set<Language::Value> HttpHeader::getContentLanguages() const
{
	return _contentLanguages;
}

HttpHeader& HttpHeader::addAccept(const MediaType::Value accept)
{
	_accepts.insert(accept);
	return *this;
}

std::set<MediaType::Value> HttpHeader::getAccepts() const
{
	return _accepts;
}

HttpHeader& HttpHeader::addAcceptLanguage(const Language::Value language)
{
	_acceptLanguages.push_back(language);
	return *this;
}

std::vector<Language::Value> HttpHeader::getAcceptLanguages() const
{
	return _acceptLanguages;
}

HttpHeader& HttpHeader::addAcceptCharset(const Charset::Value charset)
{
	_acceptCharsets.push_back(charset);
	return *this;
}

std::vector<Charset::Value> HttpHeader::getAcceptCharsets() const
{
	return _acceptCharsets;
}

HttpHeader& HttpHeader::addCookie(const Cookie &cookie)
{
	_cookies[cookie.getKey()] = cookie;
	return *this;
}

Cookie& HttpHeader::getCookie(const string &key)
{
	auto cookieIt = _cookies.find(key);
	if (cookieIt != _cookies.end()) {
		return cookieIt->second;
	}

	Cookie cookie;
	cookie.setKey(key);
	_cookies[key] = cookie;

	return _cookies[key];
}

boost::optional<Cookie const&> HttpHeader::getCookie(const string &key) const
{
	auto cookieIt = _cookies.find(key);
	if (cookieIt == _cookies.end()) {
		return boost::optional<Cookie const&>();
	}

	return boost::optional<Cookie const&>(cookieIt->second);
}

std::map<string, Cookie> const& HttpHeader::getCookies() const
{
	return _cookies;
}

HttpHeader& HttpHeader::clearCookies()
{
	_cookies.clear();
	return *this;
}

HttpHeader& HttpHeader::clear()
{
	_status = std::make_pair<Status::Value, string>(Status::UNDEFINED, "");
	_method = Method::UNDEFINED;
	_location.clear();
	_contentType = MediaType::UNDEFINED;
	_contentCharset = Charset::UNDEFINED;
	_contentBoundary.clear();
	_contentLanguages.clear();
	_accepts.clear();
	_acceptLanguages.clear();
	_acceptCharsets.clear();
	_cookies.clear();
	return *this;
}

string HttpHeader::toString(const unsigned int contentSize) const
{
	string header("");

	if (_status.first != Status::UNDEFINED) {
		header += "Status: " + boost::lexical_cast<string>(_status.first) + " " + 
			_status.second + EOL;
	}

	if (_location.empty() == false) {
		header += "Location: " + _location + EOL;
	}

	if (_contentType != MediaType::UNDEFINED) {
		header += "Content-Type: " + MediaType::toString(_contentType);

		if (_contentCharset != Charset::UNDEFINED) {
			header += "; charset=" + Charset::toString(_contentCharset);
		}

		if (_contentBoundary.empty() == false) {
			header += "; boundary=" + _contentBoundary;
		}

		header += EOL;
	}
	
	if (contentSize > 0) {
		header += "Content-Length: " + 
			boost::lexical_cast<string>(contentSize) + EOL;
	}

	if (_contentLanguages.empty() == false) {
		header += "Content-Language: ";
		for (Language::Value language : _contentLanguages) {
			header += Language::toString(language) + ",";
		}
		header = header.substr(0, header.size() - 1) + EOL;
	}

	if (_accepts.empty() == false) {
		header += "Accept: ";
		for (MediaType::Value accept : _accepts) {
			header += MediaType::toString(accept) + ",";
		}
		header = header.substr(0, header.size() - 1) + EOL;
	}

	if (_acceptLanguages.empty() == false) {
		header += "Accept-Language: ";
		for (Language::Value language : _acceptLanguages) {
			header += Language::toString(language) + ",";
		}
		header = header.substr(0, header.size() - 1) + EOL;
	}

	if (_acceptCharsets.empty() == false) {
		header += "Accept-Charset: ";
		for (Charset::Value charset : _acceptCharsets) {
			header += Charset::toString(charset) + ",";
		}
		header = header.substr(0, header.size() - 1) + EOL;
	}

	for (auto cookie: _cookies) {
		header += cookie.second.build() + EOL;
	}

	header += EOL;

	return header;
}

CGIPLUS_NS_END
