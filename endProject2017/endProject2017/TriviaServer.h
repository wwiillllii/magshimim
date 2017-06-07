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
	void serve(); 
	int nextRoomId();

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
	void bindAndListen();
	void messageHandler();
	void clientHandler(SOCKET client);
	User* getUserBySocket(SOCKET socket);
	/*
	void serve();
private:
	void accept();
	void clientHandler(SOCKET client_socket);

	Room* getRoomById(int roomId);
	User* getUserByName(std::string username);
	User* getUserBySocket(SOCKET client_socket);

	void handleRecievedMessages();
	void safeDeleteUser(RecievedMessage* msg);

	User* handleSignin(RecievedMessage* msg);
	void handleSignout(RecievedMessage* msg);
	bool handleSignup(RecievedMessage* msg);

	void handleLeaveGame(RecievedMessage* msg);
	void handleStartGame(RecievedMessage* msg);

	void handlePlayerAnswer(RecievedMessage* msg);
	void handleGetBestScores(RecievedMessage* msg);
	void handleGetPersonalStatus(RecievedMessage* msg);

	bool handleCreateRoom(RecievedMessage* msg);
	bool handleCloseRoom(RecievedMessage* msg);
	bool handleJoinRoom(RecievedMessage* msg);
	bool handleLeaveRoom(RecievedMessage* msg);
	void handleGetUsersInRoom(RecievedMessage* msg);
	void handleGetRooms(RecievedMessage* msg);

	void addRecievedMessage(RecievedMessage* msg);
	void buildRecievedMessage(SOCKET client_socket, int msgCode);
	*/
};