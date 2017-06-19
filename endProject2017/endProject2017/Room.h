#pragma once

#include <string>
#include <vector>
#include <map>

#include "User.h"
#include "Database.h"
#include "Question.h"

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

	void startGame(DataBase& db);
	bool isGame();
	void answer(User* u, int a, int t);
	bool nextQuestion();
	
private:
	void updateUsers();

	std::string name;
	// Map users to pairs of <has user answered?, score>
	std::map<User*, std::pair<bool,int>> users;
	std::vector<Question> questions;
	User* admin;
	int maxUsers;
	int secondsPerQuestion;
	int numQuestions;
	int questionIndex;
	bool _isGame;
};