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

#ifndef __CGIPLUS_MEDIATYPE_HPP__
#define __CGIPLUS_MEDIATYPE_HPP__

#include <string>

#include "Cgiplus.hpp"

using std::string;

CGIPLUS_NS_BEGIN

/*! \class MediaType
 *  \brief Store all possible media types
 */
class MediaType
{
public:
	/*! Media types
	 */
	enum Value {
		ANY,
		UNKNOWN,
		APPLICATION_ANY,
		APPLICATION_JSON,
		APPLICATION_XML,
		APPLICATION_X_WWW_FORM_URL_ENCODED,
		MULTIPART_ANY,
		MULTIPART_FORM_DATA,
		TEXT_ANY,
		TEXT_HTML,
		TEXT_PLAIN,
		TEXT_XML
	};

	static Value detect(const string &value);
};

CGIPLUS_NS_END

#endif // __CGIPLUS_MEDIATYPE_HPP__
