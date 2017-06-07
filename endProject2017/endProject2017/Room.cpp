#include <algorithm>
#include <iostream>

#include "Room.h"
#include "Helper.h"

Room::Room(User* admin, std::string name, int maxUsers, int questionsNum, int questionTime)
{
	if (!admin ||
		maxUsers < 1 || maxUsers > 9 ||
		questionsNum < 1 || questionsNum > 99 ||
		questionTime < 1 || questionTime > 99)
		throw exception("Illigal values in room constructor!");
	this->admin = admin;
	this->name = name;
	this->maxUsers = maxUsers;
	this->numQuestions = questionsNum;
	this->secondsPerQuestion = questionTime;
	users.push_back(admin);
}

void Room::addUser(User* user)
{
	users.push_back(user);
}

void Room::removeUser(User* user)
{
	std::vector<User*>::iterator it = std::find(users.begin(), users.end(), user);
	if (it == users.end())
	{
		std::cerr << "Error: user to be removed is not in this room!" << std::endl;
	}
	else users.erase(it);
	user->setRoom(NULL);
}

void Room::close()
{
	std::vector<User*>::iterator it = users.begin();
	while (it != users.end())
	{
		Helper::sendData((*it)->getSocket(), "116");
		(*it)->setRoom(NULL);
		it++;
	}
}

std::string Room::getName() const
{
	return name;
}
int Room::getUsersCount() const
{
	return users.size();
}
int Room::getMaxUsers() const
{
	return maxUsers;
}
int Room::getSecondsPerQuestion() const
{
	return secondsPerQuestion;
}
int Room::getNumQuestions() const
{
	return numQuestions;
}
std::string Room::getUserNames()
{
	std::string ret = "";
	std::vector<User*>::iterator it = users.begin();
	while (it != users.end())
	{
		ret += Helper::getPaddedNumber((*it)->getName().size(),2);
		ret += (*it)->getName();
		it++;
	}
	return ret;
}