#include "User.h"

User::User(std::string name, SOCKET socket)
{
	this->name = name;
	this->socket = socket;
}

std::string User::getName() const
{
	return this->name;
}
Room* User::getRoom() const
{
	return room;
}
void User::setRoom(Room* room)
{
	this->room = room;
}
SOCKET User::getSocket() const
{
	return socket;
}