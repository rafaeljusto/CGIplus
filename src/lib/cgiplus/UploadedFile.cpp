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

extern "C" {
#include <unistd.h>
}

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <cgiplus/UploadedFile.hpp>

CGIPLUS_NS_BEGIN

UploadedFile::UploadedFile() :
	_filename(""),
	_controlName("")
{
}

void UploadedFile::setMultipart(const string &multipart)
{
	// Each part is expected to contain "Content-disposition"
	bool dispositionFound = false;

	std::stringstream ss(multipart);
	string line;

	// Try to find headers
	while (std::getline(ss, line)) {
		if (line.find("Content-") != string::npos) {
			// Content-disposition is a special case
			if (line.find("Content-Disposition") != string::npos) {
				dispositionFound = true;
			}

			parseContentHeader(line);
			continue;
		}

		// CRLF is used to separate lines of data
		boost::trim(line);

		// Content-disposition is mandatory
		if (line.empty() && dispositionFound) {
			break;
		}
	}

	// Payload ends with "--"
	int begin = ss.tellg();
	int end = multipart.find_last_of("--") - ss.tellg() - 1;

	// Write data into file
	generateRandomFilename();
	std::ofstream file(_filename); // TODO: Write binary?
	file << multipart.substr(begin, end);
	file.close();
}

string UploadedFile::getFilename() const
{
	return _filename;
}

string UploadedFile::getControlName() const
{
	return _controlName;
}

void UploadedFile::parseContentHeader(const string &contentHeader)
{
	std::vector<string> keysValues;
	boost::split(keysValues, contentHeader, boost::is_any_of(":"));

	if (keysValues.size() != 2 || keysValues[0] != "Content-Disposition") {
		return;
	}

	boost::split(keysValues, keysValues[1], boost::is_any_of(";"));

	for (auto keyValue: keysValues) {
		boost::trim(keyValue);

		if (boost::starts_with(keyValue, "name=")) {
			size_t pos = keyValue.find("=");
			_controlName = keyValue.substr(pos + 2, keyValue.size() - pos - 3);
			
		} else if (boost::starts_with(keyValue,"filename=")) {
			size_t pos = keyValue.find("=");
			_filename = keyValue.substr(pos + 2, keyValue.size() - pos - 3);
		}
	}
}

void UploadedFile::generateRandomFilename()
{
	// For now we are ignoring the current filename because this field
	// is optional in upload parameters.

	char filename[21] = "uploaded_file-XXXXXX";

	int fd = mkstemp(filename);
	if (fd == -1) {
		return;
	}
	close(fd);

	_filename = static_cast<string>(filename);
}

CGIPLUS_NS_END
