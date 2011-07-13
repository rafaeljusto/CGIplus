/*
  CGIplus Copyright (C) 2011 Rafael Dantas Justo

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

#ifndef __CGIPLUS_COOKIE_H__
#define __CGIPLUS_COOKIE_H__

#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "Cgiplus.hpp"

using std::string;

CGIPLUS_NS_BEGIN

class Cookie
{
public:
	Cookie();

	string build() const;

	Cookie& setDomain(const string &domain);
	Cookie& setPath(const string &path);
	Cookie& setKey(const string &key);
	Cookie& setValue(const string &value);
	Cookie& setSecure(const bool secure);
	Cookie& setHttpOnly(const bool httpOnly);
	Cookie& setExpiration(const unsigned int seconds);

private:
	string _domain;
	string _path;
	string _key;
	string _value;
	bool _secure;
	bool _httpOnly;
	boost::posix_time::ptime _expiration;
};

CGIPLUS_NS_END

#endif // __CGIPLUS_COOKIE_H__
