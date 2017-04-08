This is for part 3 (RSA).
To add RSA functionality, every client has to:
1)	Generate his own key pair
2)	Send his public key to other clients
3)	keep a list of the public keys of other clients
4)	Ignore messages which are not meant for him
5)	Decrypt messages using his own private key
6)	Optional: Sign every message using his own private key
To do that, a few additions to the protocol are required:
1)	A protocol for broadcasting a key. for example, a meesage like:
	[client name] : RSA public key : [public key]
	When a message of this type is recieved, a client doesn't display the message.
	Instead, the public key is added to the list.
2)	A protocol to send RSA encrypted messages, for example:
	[client name] : RSA encrypted for [destination name] : [encrypted message]
	When a client recieves this message, he ignores it if he is not the destination.
	Otherwise, he decrypts the message using his private key and prints it to the screen.
3)	A protocol to request an agent's public key, for example:
	[client name] : RSA key request [destination name]
	This is required when a client wants to send a private message to another client, but doesn't
	know his public key. When a client recieves this message, he broadcasts his public key if he is
	the destination of the message.
