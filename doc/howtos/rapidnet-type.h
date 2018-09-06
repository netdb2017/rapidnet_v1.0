/*!
\page rapidnet-type How to add a new RapidNet value type?
\anchor rapidnet-type

<b>Question:</b> How to add a new RapidNet value type?

<b>Answer:</b> Follow these steps to add a new type to RapidNet:

Library changes:
  - Implement a value class that extends Value. Refer to existing
    types for examples. Lots of rules for implementing are straightforward
    but implicit and also necessary for your type to work correctly. Also,
    avoid picking a name that clashes with an existing ns3 class. This is
    the reason why DoubleValue is called RealValue and StringValue is called
    StrValue in RapidNet.
  - Include its header in rapidnet-types.h
  - Add a new enum entry in \ref ValueTypeId
  - Update \ref GetTypeName() function to return a string name for the type.
  - Update the \ref GetValueForType() function to return a default instance.
  - Add a test method to src/rapidnet/tests/values-test.cc to test
    Equals, Less and Clone methods of your new type.
  - Updates src/rapidnet/tests/header-test.cc to test serialization methods.
  - Update src/rapidnet/tests/expression-test.cc to test the Eval method.
  - Run the ./rapidnet/tests/testall script to make sure all tests are passing.
  - Implement the rn_<type> macro for convenient extraction of the value underneath
    (unboxing).

Compiler changes:
  - Update function \ref LookupRapidNetTypeCode() to map the type-name used in NDlog to
    the type-name string used in \ref GetTypeName()
  - Update function \ref LookupRapidNetTypeCode() to map the type-code in
    in ValuePtr to the type-name used in \ref GetTypeName()
  - Update function \ref LookupRapidNetValueClass() to map the type-code in ValuePtr
    to the corresponding value class.

  General Notes:
  - You are free to add type specific functions to your value class. But make sure
     that the value object is immutable. This can be done by declaring type specific
     functions as const. See ListValue for examples. If your type is mutable then things
     may break as this is assumed in the library.

  Notes on serialization:
  - All values must call Value::GetSerializedSize to include the typeid of the value
  - All value must call Value::Serialize to serialize the typeid of the value
  - All values can assume that the typeid has been already Deserialized and removed
    before the Deserialize of the respective value is invoked.

*/
