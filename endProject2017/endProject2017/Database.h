#pragma once

#include <vector>

#include "sqlite3.h"
#include "Question.h"

class DataBase
{
public:
	DataBase();
	// Try to sign in. returns true if succesful.
	bool signin(std::string name, std::string pass);
	// Signup. Returns 0 if succesful.
	int signup(std::string name, std::string pass, std::string mail);

	// Fill v with n randomly picked questions.
	void generateQuestions(std::vector<Question>* v, int n);
	// Register a new game. Returns its id in the database.
	int newGame();
	// End a game.
	void endGame(int gameId);
	// Register a user's answer.
	void answer(int gameId, const std::string& Name, int qId, const std::string&  a, int correct, int time);
	// Returns a vector pairing usernames and their scores. (Top 3)
	std::vector<std::pair<std::string, int>> getHighScores();
	// Returns a string describing the personal status of a player.
	std::string getPersonalStatus(const std::string& name);
private:
	sqlite3* db;
};