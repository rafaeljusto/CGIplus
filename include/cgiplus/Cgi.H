#ifndef __CGIPLUS_CGI_H__
#define __CGIPLUS_CGI_H__

#include <map>
#include <string>

#include <boost/lexical_cast.hpp>

#include "Cgiplus.H"

using std::string;

CGIPLUS_NS_BEGIN

class Cgi
{
public:
	class Method
	{
	public:
		enum Valor {
			UNKNOWN,
			GET,
			POST
		};
	};

	Cgi();

	string operator[](const string &chave);

	void readInputs();

	Method::Valor method() const;
	unsigned int numberOfInputs() const;

	void show(const std::map<string, string> &inputs,
	          const string &form) const;

private:
	void readMethod();
	void readGetInputs();
	void readPostInputs();

	void parse(string inputs);

	void decode(string &inputs);
	void decodeSpecialSymbols(string &inputs);
	void decodeHexadecimal(string &inputs);
	string hexadecimalToText(const string &hexadecimal);
	void removeDangerousHtmlCharacters(string &inputs);

	Method::Valor _method;
	std::map<string, string> _inputs;
};

CGIPLUS_NS_END

#endif // __CGIPLUS_CGI_H__
