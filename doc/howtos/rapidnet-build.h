/*!
\page rapidnet-build How to build RapidNet?
\anchor rapidnet-build

<b>Question:</b> How to build RapidNet?

<b>Answer:</b> To build RapidNet you need to install a few pre-requisite
libraries. Follow these steps:

- Installing pre-requisites for RapidNet:
   - GNU g++ 4.3.2 or later
   - Flex 2.5
   - GNU Bison 2.3
   - OpenSSL Development Library
   - <a href="http://gmplib.org/">The GNU MP Bignum Library</a> or GMP
- Download RapidNet from the
  <a href="http://netdb.cis.upenn.edu/rapidnet/downloads.html">downloads page</a>.
- Extract the contents and run <b>./waf</b> to build
- Run <b>./rapidnet/tests/testall</b> to run ns-3 and RapidNet unit test-cases.

To install and run the RapidNet visualizer, follow these steps:

- Installing pre-requisites for RapidNet visualizer:
   - Install PyGtk
   - Install Matplotlib
   - Install PyGtkCodeBuffer from <a href="http://code.google.com/p/pygtkcodebuffer/">here</a>.
   - Install Gmpy - GMP for Python
- Download RapidNet visualizer from the
  <a href="http://netdb.cis.upenn.edu/rapidnet/downloads.html">downloads page</a>.
- Extract and run ./runner.py to launch the visualizer.
*/
