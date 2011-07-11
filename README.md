CGIplus
=======

**CGIplus** is a C++ tiny web framework to parse Apache requests. The
source code was written in english, except for tests were written in
Portuguese.

Pre-Requisits
-------------

  * g++ 4.6
  * python
  * scons
  * libboost-system-dev
  * libboost-test-dev
  * libboost-regex-dev

Installation
------------

<pre>
  # scons
  # scons install
</pre>

Usage
-----

<pre><code>
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
</code></pre>

Contact
-------

  Rafael Dantas Justo
  adm@rafael.net.br
