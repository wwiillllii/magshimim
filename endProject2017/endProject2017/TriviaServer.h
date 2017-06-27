#include <iostream>
#include <WinSock2.h>
#include <map>

#include "sqlite3.h"

#include "Room.h"
#include "User.h"
#include "Database.h"
#include "RecievedMessage.h"

#define PORT 8820

class TriviaServer
{
private:
	SOCKET socket;
	std::map<SOCKET, User*> connectedUsers;
	DataBase db;
	std::map<int, Room*> rooms;
public:
	TriviaServer();
	~TriviaServer();
	// Start the server.
	void serve(); 

	void handle200(RecievedMessage& msg);
	void handle201(RecievedMessage& msg);
	void handle203(RecievedMessage& msg);
	void handle205(RecievedMessage& msg);
	void handle207(RecievedMessage& msg);
	void handle209(RecievedMessage& msg);
	void handle211(RecievedMessage& msg);
	void handle213(RecievedMessage& msg);
	void handle215(RecievedMessage& msg);
	void handle217(RecievedMessage& msg);
	void handle219(RecievedMessage& msg);
	void handle222(RecievedMessage& msg);
	void handle223(RecievedMessage& msg);
	void handle225(RecievedMessage& msg);
	void handle299(RecievedMessage& msg);
private:
	int nextRoomId();
	void bindAndListen();
	void messageHandler();
	void clientHandler(SOCKET client);
	void gameClock(Room* r);
	User* getUserBySocket(SOCKET socket);
};