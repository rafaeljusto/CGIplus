#include <cstdlib>
#include <iostream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>

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

	const char *entradasPtr;

	entradasPtr = getenv("QUERY_STRING");
	if (entradasPtr != NULL) {
		string entradas = entradasPtr;
		interpretarEntradas(entradas);
	}

	entradasPtr = getenv("CONTENT_LENGTH");
	if (_metodo == Metodo::POST && entradasPtr != NULL) {
		int tamanho = 0;
		try {
			tamanho = boost::lexical_cast<int>(entradasPtr);
		} catch (const  boost::bad_lexical_cast &e) {}

		if (tamanho > 0) {
			char entradas[tamanho + 1];
			memset(entradas, 0, tamanho + 1);
			std::cin.read(entradas, tamanho);
			interpretarEntradas((string) entradas);
		}
	}
}

string Cgi::operator[](const string &chave)
{
	auto entrada = _entradas.find(chave);
	if (entrada != _entradas.end()) {
		return entrada->second;
	}

	return "";
}

Cgi::Metodo::Valor Cgi::getMetodo() const
{
	return _metodo;
}

unsigned int Cgi::quantidadeEntradas() const
{
	return _entradas.size();
}

void Cgi::exibir(const map<string, string> &entradas,
                 const string &modelo) const
{
	string conteudo = modelo;
	for (auto entrada: entradas) {
		boost::replace_all(conteudo, entrada.first, entrada.second);
	}
	
	std::cout << "Content-type: text/html\n\r\n\r" << std::endl;
	std::cout << conteudo << std::endl;
}

void Cgi::interpretarEntradas(string entradas)
{
	boost::trim(entradas);
	boost::replace_all(entradas, "+", " ");
	
	std::vector<string> chavesValores;
	boost::split(chavesValores, entradas, boost::is_any_of("&"));

	for (auto chaveValor: chavesValores) {
		std::vector<string> chaveValorSeparado;
		boost::split(chaveValorSeparado, chaveValor, boost::is_any_of("="));
		if (chaveValorSeparado.size() == 2) {
			_entradas[chaveValorSeparado[0]] = chaveValorSeparado[1];
		}
	}
}

CGIPLUS_NS_END
