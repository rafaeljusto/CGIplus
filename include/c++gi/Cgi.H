#ifndef __CPPGI_CGI_H__
#define __CPPGI_CGI_H__

#include <map>
#include <string>

#include "Cppgi.H"

using std::map;
using std::string;

CPPGI_NS_BEGIN

class Cgi
{
public:
	class Metodo
	{
	public:
		enum Valor {
			DESCONHECIDO,
			GET,
			POST
		};
	};

	Cgi();

	string operator[](const string &chave);

	Metodo::Valor getMetodo() const;

	void exibir(const map<string, string> &valores,
	            const string &modelo) const;

private:
	Metodo::Valor _metodo;
	map<string, string> _valores;
};

CPPGI_NS_END

#endif // __CPPGI_CGI_H__
