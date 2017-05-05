/*!
\page rapidnet-function How to add a new user defined function?
\anchor rapidnet-function

<b>Question:</b> How to add a new user defined function?

<b>Answer:</b> Follow these steps:

Library changes:
  - Define a function class extending FunctionExpr in rapidnet-functions.h/cc
    Make sure it is generic by taking in arguments of type \ref Expression wherever
    possible.
  - Write test cases in rapidnet/tests/functions-test.cc

Compiler changes:
  - Update the function \ref LookupFunction() to map the function name to be used
    in the NDlog program to its class name in the Library.
*/
