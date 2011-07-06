#include <sstream>

#include <boost/date_time/gregorian/gregorian.hpp>

#include <cgiplus/Cookie.hpp>

CGIPLUS_NS_BEGIN

Cookie::Cookie() :
	_domain(""),
	_path(""),
	_key(""),
	_value(""),
	_secure(false),
	_httpOnly(false)
{
}

Cookie& Cookie::setDomain(const string &domain)
{
	_domain = domain;
	return *this;
}

Cookie& Cookie::setPath(const string &path)
{
	_path = path;
	return *this;
}

Cookie& Cookie::setKey(const string &key)
{
	_key = key;
	return *this;
}

Cookie& Cookie::setValue(const string &value)
{
	_value = value;
	return *this;
}

Cookie& Cookie::setSecure(const bool secure)
{
	_secure = secure;
	return *this;
}

Cookie& Cookie::setHttpOnly(const bool httpOnly)
{
	_httpOnly = httpOnly;
	return *this;
}

Cookie& Cookie::setExpiration(const unsigned int seconds)
{
	_expiration = boost::posix_time::second_clock::universal_time();
	_expiration += boost::posix_time::seconds(seconds);
	return *this;
}

string Cookie::build() const
{
	if (_key.empty()) {
		return "";
	}

	string cookie = "Set-Cookie: " + _key + "=" + _value + "; ";

	if (_expiration != boost::posix_time::not_a_date_time) {
		boost::gregorian::date_facet *facetDate = 
			new boost::gregorian::date_facet("%A, %d-%b-%y ");

		std::stringstream expirationDate;
		expirationDate.imbue(std::locale(std::locale::classic(), facetDate));
		expirationDate << _expiration.date();

		boost::gregorian::date_facet *facetTime = 
			new boost::gregorian::date_facet("%T GMT");

		std::stringstream expirationTime;
		expirationDate.imbue(std::locale(std::locale::classic(), facetTime));
		expirationDate << _expiration.time_of_day();

		cookie += "Expires=" + expirationDate.str() + 
			expirationTime.str() + "; ";
	}

	if (_domain.empty() == false) {
		cookie += "Domain=" + _domain + "; ";
	}

	if (_path.empty() == false) {
		cookie += "Path=" + _path + "; ";
	}

	if (_secure) {
		cookie += "Secure; ";
	}

	if (_httpOnly) {
		cookie += "httponly; ";
	}

	return cookie;
}

CGIPLUS_NS_END
