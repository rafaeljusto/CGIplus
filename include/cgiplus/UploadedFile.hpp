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

#ifndef __CGIPLUS_UPLOADED_FILE_H__
#define __CGIPLUS_UPLOADED_FILE_H__

#include <string>

#include "Cgiplus.hpp"

using std::string;

CGIPLUS_NS_BEGIN

/*! \class UploadedFile
 *  \brief Store uploaded data
 *
 * When sending multipart/form-data, this object is responsable for
 * storing the uploaded file.
 */
class UploadedFile
{
public:
	/*! Nothing special here, just initializing everything.
	 */
	UploadedFile();

	/*! Parse multipart according to [1] and fill all attributes.
	 * [1] http://www.w3.org/TR/html401/interact/forms.html#h-17.13.4
	 *
	 * @param multipart Raw http sent data
	 */
	void setMultipart(const string &multipart);

	/*! Returns desired filename.
	 *
	 * @return Filename
	 */
	string getFilename() const;

	/*! Returns the name of the field in the HTML form
	 *
	 * @return HTML field name
	 */
	string getControlName() const;

private:
	void parseContentHeader(const string &contentHeader);
	void generateRandomFilename();

	string _filename;
	string _controlName;
};

CGIPLUS_NS_END

#endif // __CGIPLUS_UPLOADED_FILE_H__
