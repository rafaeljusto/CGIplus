CGIplus
=======

**CGIplus** is a C++ tiny web framework to parse Apache requests. Some
features that you will find in this framework:

  * Easy interface - use [] and () operators to get (Cgi) or define
    (Builder) GET/POST data and Cookie respectively

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

  The project was compiled using the above compilers and libraries,
  higher versions should work well.

Installation
------------

    # scons
    # scons install

  For API documentation:

    # scons doc

Usage
-----

    #include <cstdlib>
    
    #include <cgiplus/Builder.hpp>
    #include <cgiplus/Cgi.hpp>
    
    int main()
    {
      // Simulating inputs
      setenv("QUERY_STRING", "test=abc123", 1);
      setenv("REQUEST_METHOD", "GET", 1);
      
      string form = "The test has as content: <!-- content -->";
      
      cgiplus::Cgi cgi;
      cgiplus::Builder builder;
      
      builder.setForm(form);
      builder["content"] = cgi["test"]
      builder.show();

      return 0;
    }

Contact
-------

  Rafael Dantas Justo  
  <adm@rafael.net.br>

License
-------

    CGIplus Copyright (C) 2011 Rafael Dantas Justo

    This file is part of CGIplus.

    CGIplus is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CGIplus is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CGIplus.  If not, see <http://www.gnu.org/licenses/>.
