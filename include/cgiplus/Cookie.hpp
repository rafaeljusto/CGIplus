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

#ifndef __CGIPLUS_COOKIE_H__
#define __CGIPLUS_COOKIE_H__

#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "Cgiplus.hpp"

using std::string;

CGIPLUS_NS_BEGIN

/*! \class Cookie
 *  \brief Store all data related to a cookie
 *
 * Use this class to create a cookie with all necessary parameters
 */
class Cookie
{
public:
	/*! Nothing special here, just initializing everything.
	 */
	Cookie();

	/*! Generate html header format text to set this cookie. It's not
	 * allowed to generate a cookie with an empty key. On any error it
	 * will return an empty string.
	 *
	 * @return Html header text
	 */
	string build() const;

	/*! Sets cookie's domain (RFC 6265 - Section 4.1.2.3). By default is
	 * empty.
	 *
	 * @param domain Cookie's domain
	 * @return Reference to the current object, allowing easy usability
	 */
	Cookie& setDomain(const string &domain);

	/*! Sets cookie's path (RFC 6265 - Section 4.1.2.4). By default is
	 * empty.
	 *
	 * @param path Cookie's path
	 * @return Reference to the current object, allowing easy usability
	 */
	Cookie& setPath(const string &path);

	/*! Sets cookie's key to identified your cookie. By default is
	 * empty.
	 *
	 * @param key Cookie's key
	 * @return Reference to the current object, allowing easy usability
	 */
	Cookie& setKey(const string &key);

	/*! Returns cookie's key
	 *
	 * @return Cookie's key
	 */
	string getKey() const;

	/*! Sets cookie's value. By default is empty.
	 *
	 * @param value Cookie's value
	 * @return Reference to the current object, allowing easy usability
	 */
	Cookie& setValue(const string &value);

	/*! Returns cookie's value
	 *
	 * @return Cookie's value
	 */
	string getValue() const;

	/*! Sets cookie's Secure flag (RFC 6265 - Section 4.1.2.5). By default is
	 * false.
	 *
	 * @param secure Cookie's Secure flag
	 * @return Reference to the current object, allowing easy usability
	 */
	Cookie& setSecure(const bool secure);

	/*! Sets cookie's HttpOnly flag (RFC 6265 - Section 4.1.2.5). By
	 * default is false.
	 *
	 * @param httpOnly Cookie's HttpOnly flag
	 * @return Reference to the current object, allowing easy usability
	 */
	Cookie& setHttpOnly(const bool httpOnly);

	/*! Sets cookie's expiration date (RFC 6265 - Section 4.1.2.1). By
	 * default is undefined (will not be in set-cookie).
	 *
	 * @param seconds Cookie's expiration date in seconds from now
	 * @return Reference to the current object, allowing easy usability
	 */
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
