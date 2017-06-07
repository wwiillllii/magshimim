#pragma once

#include "sqlite3.h"

class DataBase
{
public:
	DataBase();
	bool signin(std::string name, std::string pass);
	int signup(std::string name, std::string pass, std::string mail);
private:
	sqlite3* db;
};