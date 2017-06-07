#pragma once

#include <string>
#include <vector>

#include "User.h"

class User;

class Room
{
public:
	Room(User* admin, std::string name, int maxUsers, int questionsNum, int questionTime);

	void addUser(User* user);
	void removeUser(User* user);
	void close();

	std::string getName() const;
	int getUsersCount() const;
	int getMaxUsers() const;
	int getSecondsPerQuestion() const;
	int getNumQuestions() const;
	std::string getUserNames();
	
private:
	std::string name;
	std::vector<User*> users;
	User* admin;
	int maxUsers;
	int secondsPerQuestion;
	int numQuestions;
};