#include <cstdlib>
#include <iostream>
#include <vector>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>

#include <cgiplus/Cgi.H>

CGIPLUS_NS_BEGIN

Cgi::Cgi() :
	_metodo(Metodo::DESCONHECIDO)
{
	string metodo = getenv("REQUEST_METHOD");
	if (metodo == "GET") {
		_metodo = Metodo::GET;
	} else if (metodo == "POST") {
		_metodo = Metodo::POST;
	}

	string valores = getenv("QUERY_STRING");
	boost::replace_all(valores, "+", " ");
	
	std::vector<string> chavesValores;
	boost::split(chavesValores, valores, boost::is_any_of("&"));

	for (auto chaveValor: chavesValores) {
		std::vector<string> chaveValorSeparado;
		boost::split(chaveValorSeparado, chaveValor, boost::is_any_of("="));
		if (chaveValorSeparado.size() == 2) {
			_valores[chaveValorSeparado[0]] = chaveValorSeparado[1];
		}
	}
}

string Cgi::operator[](const string &chave)
{
	auto valor = _valores.find(chave);
	if (valor != _valores.end()) {
		return valor->second;
	}

	return "";
}

Cgi::Metodo::Valor Cgi::getMetodo() const
{
	return _metodo;
}

void Cgi::exibir(const map<string, string> &valores,
                 const string &modelo) const
{
	string conteudo = modelo;
	for (auto valor: valores) {
		boost::replace_all(conteudo, valor.first, valor.second);
	}
	
	std::cout << "Content-type: text/html\n\r\n\r" << std::endl;
	std::cout << conteudo << std::endl;
}

CGIPLUS_NS_END
