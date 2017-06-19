#include <iostream>
#include <exception>
#include <map>
#include <vector>

#include "Database.h"

DataBase::DataBase()
{
	if (sqlite3_open("trivia.db", &db))
	{
		std::cout << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		throw std::exception("Database could not be opened!");
	}
}
int getValuesMap(void* d, int n, char** values, char** columns)
{
	int i = 0;
	std::map<std::string, std::string>* data = (std::map<std::string, std::string>*)d;
	for (i = 0; i < n; i++)
		if (data->count(std::string(columns[i])))
			(*data)[columns[i]] = values[i];
	return 0;
}
bool DataBase::signin(std::string name, std::string pass)
{
	std::string q = "SELECT * FROM t_users WHERE username='" + name + "' AND password='" + pass + "';";
	std::map<std::string, std::string> data;
	data["username"] = "";
	sqlite3_exec(db, q.c_str(), getValuesMap, &data, NULL);
	return data["username"] != "";
}

bool isUsernameValid(std::string name)
{
	if (name == "") return false;
	if (!((name[0] >= 'a' && name[0] <= 'z') ||
		(name[0] >= 'A' && name[0] <= 'Z'))) return false;
	return name.find(' ') == std::string::npos;
}
bool isPasswordValid(std::string pass)
{
	if (pass.size() < 4) return false;
	if (pass.find(' ') != std::string::npos) return false;
	if (pass.find_first_of("0123456789") == std::string::npos) return false;
	if (pass.find_first_of("abcdefghijklmnopqrstuvwxyz") == std::string::npos) return false;
	if (pass.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == std::string::npos) return false;
	return true;
}
int DataBase::signup(std::string name, std::string pass, std::string mail)
{
	if (!isUsernameValid(name)) return 3;
	if (!isPasswordValid(pass)) return 1;
	std::string q = "SELECT * FROM t_users WHERE username='" + name + "';";
	std::map<std::string, std::string> data;
	data["username"] = "";
	sqlite3_exec(db, q.c_str(), getValuesMap, &data, NULL);
	if (data["username"] != "") return 2;
	q = "INSERT INTO t_users(username,password,email) VALUES (";
	q += "'" + name + "', ";
	q += "'" + pass + "', ";
	q += "'" + mail + "');";
	sqlite3_exec(db, q.c_str(), NULL, NULL, NULL);
	return 0;
}

int getValuesQuestions(void* d, int n, char** values, char** columns)
{
	std::string q;
	std::string a[4];
	int i = 0;
	for (i = 0; i < n; i++)
	{
		std::string str = columns[i];
		if (str[0] == 'q')
			q = values[i];
		else
			a[str[3] - '1'] = values[i];
	}
	((std::vector<Question>*) d)->push_back(Question(q, a));
	return 0;
}
void DataBase::generateQuestions(std::vector<Question>* v, int n)
{
	std::string q = "SELECT question,"
		"correct_ans as ans1, ans2, ans3, ans4 "
		"FROM t_questions ORDER BY RANDOM() LIMIT ";
	q += std::to_string(n) + ";";
	sqlite3_exec(db, q.c_str(), getValuesQuestions, v, NULL);
}