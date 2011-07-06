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
