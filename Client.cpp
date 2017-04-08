#include "Client.h"

CryptoDevice cryptoDevice;

#define MAX_LOGIN_ATTEMPTS 3
// Change these veriables for every agent!
const string salt = "salt";
const string nameHash = "2BFD54D8484D3F0A6758E7195A62D624"; // md5("username" + "salt")
const string passHash = "B305CADBB3BCE54F3AA59C64FEC00DEA"; // md5("password" + "salt")

bool login()
{
	string name, pass;
	string nameEnc, passEnc;

	int i = 0;
	for (i = 0; i < MAX_LOGIN_ATTEMPTS; i++)
	{
		cout << "Enter username: ";
		cin >> name;
		cout << "Enter password: ";
		cin >> pass;

		nameEnc = cryptoDevice.md5(name + salt);
		passEnc = cryptoDevice.md5(pass + salt);

		if (nameEnc != nameHash || passEnc != passHash)
			cout << "Wrong username or password!" << endl;
		else
			break;
	}
	return i != MAX_LOGIN_ATTEMPTS;
}

int process_client(client_type &new_client)
{
	while (1)
	{
		memset(new_client.received_message, 0, DEFAULT_BUFLEN);

		if (new_client.socket != 0)
		{


			int iResult = recv(new_client.socket, new_client.received_message, DEFAULT_BUFLEN, 0);

			string strMessage(new_client.received_message);

			// some logic, we dont want to decrypt notifications sent by the operator
			// our protocol says - ": " means notification from the operator
			size_t position = strMessage.find(": ") + 2;
			string prefix = strMessage.substr(0, position);
			string postfix = strMessage.substr(position);
			string decrypted_message;

			//this is the only notification we use right now :(
			if (postfix != "Disconnected")
				//please decrypt this part!
				decrypted_message = cryptoDevice.decryptAES(postfix);
			else
				//dont decrypt this - not classified and has not been ecrypted! 
				//trying to do so may cause errors
				decrypted_message = postfix;

			if (iResult != SOCKET_ERROR)
				cout << prefix + decrypted_message << endl;
			else
			{
				cout << "recv() failed: " << WSAGetLastError() << endl;
				break;
			}
		}
	}

	if (WSAGetLastError() == WSAECONNRESET)
		cout << "The server has disconnected" << endl;

	return 0;
}

// TODO: implement self-destruction.
void haltAndCatchFire(){}

int main()
{
	WSAData wsa_data;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	string sent_message = "";
	client_type client = { INVALID_SOCKET, -1, "" };
	int iResult = 0;
	string message;

	cout << "Starting Client...\n";

	if (!login())
	{
		cout << "Destroying device!" << endl;
		haltAndCatchFire();
		system("pause");
		return -1;
	}

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (iResult != 0) {
		cout << "WSAStartup() failed with error: " << iResult << endl;
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	cout << "Connecting...\n";

	// Resolve the server address and port
	iResult = getaddrinfo(IP_ADDRESS, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		cout << "getaddrinfo() failed with error: " << iResult << endl;
		WSACleanup();
		system("pause");
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		client.socket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (client.socket == INVALID_SOCKET) {
			cout << "socket() failed with error: " << WSAGetLastError() << endl;
			WSACleanup();
			system("pause");
			return 1;
		}

		// Connect to server.
		iResult = connect(client.socket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(client.socket);
			client.socket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (client.socket == INVALID_SOCKET) {
		cout << "Unable to connect to server!" << endl;
		WSACleanup();
		system("pause");
		return 1;
	}

	cout << "Successfully Connected" << endl;

	//Obtain id from server for this client;
	recv(client.socket, client.received_message, DEFAULT_BUFLEN, 0);
	message = client.received_message;

	if (message != "Server is full")
	{
		client.id = atoi(client.received_message);

		thread my_thread(process_client, client);

		while (1)
		{
			getline(cin, sent_message);

			//top secret! please encrypt
			string cipher = cryptoDevice.encryptAES(sent_message);

			iResult = send(client.socket, cipher.c_str(), cipher.length(), 0);

			if (iResult <= 0)
			{
				cout << "send() failed: " << WSAGetLastError() << endl;
				break;
			}


		}

		//Shutdown the connection since no more data will be sent
		my_thread.detach();
	}
	else
		cout << client.received_message << endl;

	cout << "Shutting down socket..." << endl;
	iResult = shutdown(client.socket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		cout << "shutdown() failed with error: " << WSAGetLastError() << endl;
		closesocket(client.socket);
		WSACleanup();
		system("pause");
		return 1;
	}

	closesocket(client.socket);
	WSACleanup();
	system("pause");
	return 0;
}