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

	void lerEntradas();

	Metodo::Valor getMetodo() const;
	unsigned int quantidadeEntradas() const;

	void exibir(const std::map<string, string> &entradas,
	            const string &modelo) const;

private:
	void lerMetodo();
	void lerEntradasGet();
	void lerEntradasPost();

	void interpretar(string entradas);

	void decodificar(string &entradas);
	void decodificarSimbolosEspeciais(string &entradas);
	void decodificarHexadecimal(string &entradas);
	string hexadecimalParaTexto(const string &hexadecimal);
	void removerCaracteresHtmlPerigosos(string &entradas);

	Metodo::Valor _metodo;
	std::map<string, string> _entradas;
};

CGIPLUS_NS_END

#endif // __CGIPLUS_CGI_H__
