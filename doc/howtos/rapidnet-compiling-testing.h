/*!
\page rapidnet-compiling-testing Important compiling and testing tips.
\anchor rapidnet-compiling-testing

Important compiling and testing tips:

- <b>Compiler</b>: Run ./rapidnet/compiler/compile --help for options as below
\code
Usage: compile application [options]

Options:
  -h, --help            show this help message and exit
  -b APPBASE, --base=APPBASE
                        Base application file/folder path
  -p, --provenance      Enable provenance
  -v, --verbose         
\endcode
The first argument is the source file/folder path of the application being compiled.
The compiler has two important optional parameters:
  -# --base Which takes as argument the application source file/folder path
     of another application that should be the base class of the application
     being compiled. This is very useful for combining rules from two applications
     into one application, often the base application is something like a link
     discovery application. See ./rapidnet/compiler/compileall for some examples.
     If this option is omitted, the default base class is RapidNetApplicationBase.
  -# --provenance which when used enables provenance for the application being compiled.
     Note that it should be a NDlog application, provenance for SeNDlog applications is
     not currently supported
    

- <b>Testing</b>: ./rapidnet/test/testall is the test suite that should be run frequently
  to make sure nothing is broken. It runs all unit-test cases, including those defined by
  ns-3. All RapidNet specific unit test cases are defined in src/rapidnet/tests. It also
  runs a few applications and compares the generated output with an expected output and
  makes sure they match.

- <b>Compiling all</b>: ./rapidnet/compiler/compile script is used to compile all applications
  at once. Since we are also checking in generated files for the applications, it is better
  to run this and make sure all files are up to date.
*/
