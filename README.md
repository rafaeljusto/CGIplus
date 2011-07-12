CGIplus
=======

**CGIplus** is a C++ tiny web framework to parse Apache requests. Some
features that you will find in this framework:

  * Easy interface - use [] and () to get (Cgi) or define (Builder)
    GET/POST and cookie data respectively

  * Protection against cross-site scripting - Some special symbols are
    removed from the input data

Prerequisits
------------

  * g++ 4.6 - <http://gcc.gnu.org/>
  * python 2.7 - <http://www.python.org/>
  * scons 2.0 - <http://www.scons.org/>
  * libboost-system-dev 1.4 - <http://www.boost.org>
  * libboost-test-dev 1.4 - <http://www.boost.org>
  * libboost-regex-dev 1.4 - <http://www.boost.org>

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
      
      std::cout << builder.build() << std::endl;
      return 0;
    }

Contact
-------

  Rafael Dantas Justo  
  <adm@rafael.net.br>
