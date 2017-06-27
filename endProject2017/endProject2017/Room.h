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
	Room(User* admin, std::string name, int maxUsers, int questionsNum, int questionTime, DataBase* db);

	// Add a new user to the room.
	void addUser(User* user);
	// Remve a user from the room. room is closed if user is admin.
	void removeUser(User* user);
	// Close room.
	void close();

	// Get room name.
	std::string getName() const;
	// Get the amount of users in the room.
	int getUsersCount() const;
	// Get maximum amount of users in the room.
	int getMaxUsers() const;
	// Get maximum seconds to answer a question.
	int getSecondsPerQuestion() const;
	// Get the amount of questions.
	int getNumQuestions() const;
	// Get a list of user names in the format: [length:2 bytes][name]
	std::string getUserNames();

	// Start the game
	void startGame();
	// Returns true if the game started.
	bool isGame();
	// Register a user's answer.
	void answer(User* u, int a, int t);
	// Send next question. return true if there are more questions' false if game has ended.
	bool nextQuestion();
	
private:
	// Send updated list of users to connected users.
	void updateUsers();

	std::string name;
	// Map users to pairs of <has user answered?, score>
	std::map<User*, std::pair<bool,int>> users;
	std::vector<Question> questions;
	DataBase* db;
	User* admin;
	int maxUsers;
	int secondsPerQuestion;
	int numQuestions;
	int questionIndex;
	bool _isGame;
	int gameId;
};