#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include <cgiplus/Cgi.H>

CGIPLUS_NS_BEGIN

Cgi::Cgi() :
	_metodo(Metodo::DESCONHECIDO)
{
	lerEntradas();
}

string Cgi::operator[](const string &chave)
{
	auto entrada = _entradas.find(chave);
	if (entrada != _entradas.end()) {
		return entrada->second;
	}

	return "";
}

void Cgi::lerEntradas()
{
	lerMetodo();
	lerEntradasGet();
	lerEntradasPost();
}

Cgi::Metodo::Valor Cgi::getMetodo() const
{
	return _metodo;
}

unsigned int Cgi::quantidadeEntradas() const
{
	return _entradas.size();
}

void Cgi::exibir(const std::map<string, string> &entradas,
                 const string &modelo) const
{
	string conteudo = modelo;
	for (auto entrada: entradas) {
		boost::replace_all(conteudo, entrada.first, entrada.second);
	}
	
	std::cout << "Content-type: text/html\n\r\n\r" << std::endl;
	std::cout << conteudo << std::endl;
}

void Cgi::lerMetodo()
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
}

void Cgi::lerEntradasGet()
{
	const char *entradasPtr = getenv("QUERY_STRING");
	if (entradasPtr == NULL) {
		return;
	}

	string entradas = entradasPtr;
	interpretar(entradas);
}

void Cgi::lerEntradasPost()
{
	const char *tamanhoPtr = getenv("CONTENT_LENGTH");
	if (_metodo != Metodo::POST || tamanhoPtr == NULL) {
		return;
	}

	int tamanho = 0;
	try {
		tamanho = boost::lexical_cast<int>(tamanhoPtr);
	} catch (const  boost::bad_lexical_cast &e) {}

	if (tamanho == 0) {
		return;
	}

	char entradasPtr[tamanho + 1];
	memset(entradasPtr, 0, tamanho + 1);
	
	std::cin.read(entradasPtr, tamanho);
	if (std::cin.good() == false) {
		return;
	}

	string entradas = entradasPtr;
	interpretar(entradas);
}

void Cgi::interpretar(string entradas)
{
	decodificar(entradas);

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

void Cgi::decodificar(string &entradas)
{
	boost::trim(entradas);
	decodificarSimbolosEspeciais(entradas);
	decodificarHexadecimal(entradas);
	removerCaracteresHtmlPerigosos(entradas);
}

void Cgi::decodificarSimbolosEspeciais(string &entradas)
{
	boost::replace_all(entradas, "+", " ");	
}

void Cgi::decodificarHexadecimal(string &entradas)
{
	boost::match_results<string::const_iterator> encontrado;
	boost::regex hexadecimal("%[0-9A-F][0-9A-F]");

	while (boost::regex_search(entradas, encontrado, hexadecimal)) {
		string hexadecimalTratado = boost::to_upper_copy(encontrado.str());
		string texto = hexadecimalParaTexto(hexadecimalTratado);
		boost::replace_all(entradas, encontrado.str(), texto);
	}
}

string Cgi::hexadecimalParaTexto(const string &hexadecimal)
{
	std::stringstream hexadecimalStream;
	hexadecimalStream << std::hex << hexadecimal.substr(1);

	unsigned int hexadecimalNumber = 0;
	hexadecimalStream >> hexadecimalNumber;

	string texto("");
	texto += char(hexadecimalNumber);
	return texto;
}

void Cgi::removerCaracteresHtmlPerigosos(string &entradas)
{
	boost::replace_all(entradas, "'", "");
	boost::replace_all(entradas, "\"", "");
	boost::replace_all(entradas, "<", "");
	boost::replace_all(entradas, ">", "");
}

CGIPLUS_NS_END
