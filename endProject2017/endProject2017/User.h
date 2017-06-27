#pragma once

#include <string>
#include <WinSock2.h>

#include "Room.h"

class Room;

class User
{
public:
	User(std::string name, SOCKET socket);
	// Get user name
	std::string getName() const;
	// Get user's socket
	SOCKET getSocket() const;
	// Get user's room. NULL if user is not in a room.
	Room* getRoom() const;
	// Set user's room.
	void setRoom(Room* room);
private:
	std::string name;
	Room* room;
	SOCKET socket;
};