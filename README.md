CGIplus
=======

**CGIplus** is a C++ tiny web framework to parse Apache requests. The
source code was written in english, except for tests were written in
Portuguese.

Prerequisits
------------

  * g++ 4.6 - see <http://gcc.gnu.org/>
  * python - see <http://www.python.org/>
  * scons - see <http://www.scons.org/>
  * libboost-system-dev - see <http://www.boost.org>
  * libboost-test-dev - see <http://www.boost.org>
  * libboost-regex-dev - see <http://www.boost.org>

Installation
------------

    # scons
    # scons install

Usage
-----

    #include <cstdlib>
    #include <iostream>
    
    #include <cgiplus/Builder.hpp>
    #include <cgiplus/Cgi.hpp>
    
    int main()
    {
      // Enviroment variables defined by Apache in a request
      setenv("QUERY_STRING", "test=abc123", 1);
      setenv("REQUEST_METHOD", "GET", 1);
      
      string form = "The test has as content: <!-- content -->";
      
      cgiplus::Cgi cgi;
      cgiplus::Builder builder;
      
      builder.setForm(form);
      builder["content"] = cgi["test"]
      
      std::cout \<\< builder.build() \<\< std::endl;
      return 0;
    }

Contact
-------

  Rafael Dantas Justo  
  <adm@rafael.net.br>
