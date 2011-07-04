#include <cstdlib>
#include <iostream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>

#include <cgiplus/Cgi.H>

CGIPLUS_NS_BEGIN

Cgi::Cgi() :
	_metodo(Metodo::DESCONHECIDO)
{
	const char *metodoPtr = getenv("REQUEST_METHOD");
	if (metodoPtr != NULL) {
		string metodo = boost::to_upper_copy((string) metodoPtr);
		if (metodo == "GET") {
			_metodo = Metodo::GET;
		} else if (metodo == "POST") {
			_metodo = Metodo::POST;
		}
	}

	const char *valoresPtr = getenv("QUERY_STRING");
	if (valoresPtr == NULL) {
		return;
	}

	string valores = valoresPtr;
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

unsigned int Cgi::quantidadeEntradas() const
{
	return _valores.size();
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
