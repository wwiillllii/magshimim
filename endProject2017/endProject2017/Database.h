#pragma once

#include <vector>

#include "sqlite3.h"
#include "Question.h"

class DataBase
{
public:
	DataBase();
	bool signin(std::string name, std::string pass);
	int signup(std::string name, std::string pass, std::string mail);
	void generateQuestions(std::vector<Question>* v, int n);
private:
	sqlite3* db;
};