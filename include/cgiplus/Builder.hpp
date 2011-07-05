#ifndef __CGI_PLUS_BUILDER_H__
#define __CGI_PLUS_BUILDER_H__

#include <map>
#include <string>

#include "Cgiplus.hpp"

using std::string;

CGIPLUS_NS_BEGIN

class Builder
{
public:
	class Method
	{
	public:
		enum Value {
			PLAIN_TEXT,
			HTML
		};

		static string toString(const Value value);
	};

	Builder();

	string& operator[](const string &key);

	string build() const;
	void show() const;

	void setForm(const string &form);
	void setTags(const std::pair<string, string> &tags);
	void setFormFile(const string &formFile);
	void setMethod(const Method::Value method);

private:
	string _form;
	std::pair<string, string> _tags;
	Method::Value _method;
	std::map<string, string> _fields;
};

CGIPLUS_NS_END

#endif // __CGI_PLUS_BUILDER_H__
