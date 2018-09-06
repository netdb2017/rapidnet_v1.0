/*!
\page rapidnet-sendlog-customizations How can we customize the SeNDlog features?
\anchor rapidnet-sendlog-customizations

<b>Question:</b> How can we customize the SeNDlog features?

<b>Answer:</b> SeNDlog implementation in RapidNet implements encryption and
authentication features. Both these are implemented to allow customizations.

- <b>Authentication</b>: The SendlogAuthenticationManager is the base class for all
authentication implementations. A sample and useful implementation provided for this
is the PkiAuthenticationManager which uses public and private keys for authentication.
To add a new authentication implementation, extend the SendlogAuthenticationManager
class and implement the required methods. Update the factory method
RapidNetApplicationBase::SendlogInit to create an instance of your implementation
class.

- <b>Encryption</b>:
The SendlogEncryptionManager is the base class for all encryption
implementations. A sample implementation provided for this is the
BlowfishAuthenticationManager which uses public and private keys for
authentication. Update the factory method RapidNetApplicationBase::SendlogInit
to create an instance of your implementation class. To add a new authentication
implementation, extend the SendlogEncryptionManager class and implement the
required methods.

*/
