#pragma once

#include <string>
#include <WinSock2.h>

#include "Room.h"

class Room;

class User
{
public:
	User(std::string name, SOCKET socket);
	std::string getName() const;
	SOCKET getSocket() const;
	Room* getRoom() const;
	void setRoom(Room* room);
private:
	std::string name;
	Room* room;
	SOCKET socket;
};