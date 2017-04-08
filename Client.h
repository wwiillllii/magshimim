#pragma once

#include "CryptoDevice.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512            
#define IP_ADDRESS "127.0.0.1"
#define DEFAULT_PORT "8202"
struct client_type
{
	SOCKET socket;
	int id;
	char received_message[DEFAULT_BUFLEN];
};

class Client
{
public:
	Client();
	~Client();
	int main();

private:
	int process_client(client_type  &new_client);
};

