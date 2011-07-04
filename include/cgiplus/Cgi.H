#ifndef __CGIPLUS_CGI_H__
#define __CGIPLUS_CGI_H__

#include <map>
#include <string>

#include "Cgiplus.H"

using std::map;
using std::string;

CGIPLUS_NS_BEGIN

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
	unsigned int quantidadeEntradas() const;

	void exibir(const map<string, string> &entradas,
	            const string &modelo) const;

private:
	void interpretarEntradas(string entradas);

	Metodo::Valor _metodo;
	map<string, string> _entradas;
};

CGIPLUS_NS_END

#endif // __CGIPLUS_CGI_H__
