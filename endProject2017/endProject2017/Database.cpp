#include <iostream>
#include <exception>
#include <map>
#include <vector>

#include "MD5.h"
#include "Database.h"
#include "Helper.h"

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
	std::string q = "SELECT * FROM t_users WHERE username='" + name + "' AND password='" + MD5(pass) + "';";
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
	q += "'" + MD5(pass) + "', ";
	q += "'" + mail + "');";
	sqlite3_exec(db, q.c_str(), NULL, NULL, NULL);
	return 0;
}

int getValuesQuestions(void* d, int n, char** values, char** columns)
{
	std::string q;
	std::string a[4];
	int qId;
	int i = 0;
	for (i = 0; i < n; i++)
	{
		std::string str = columns[i];
		if (str[0] == 'q')
			if (str[1] == 'u')
				q = values[i];
			else
				qId = std::stoi(values[i]);
		else
			a[str[3] - '1'] = values[i];
	}
	((std::vector<Question>*) d)->push_back(Question(q, a, qId));
	return 0;
}
void DataBase::generateQuestions(std::vector<Question>* v, int n)
{
	std::string q = "SELECT question,"
		"correct_ans as ans1, ans2, ans3, ans4, question_id as qId "
		"FROM t_questions ORDER BY RANDOM() LIMIT ";
	q += std::to_string(n) + ";";
	sqlite3_exec(db, q.c_str(), getValuesQuestions, v, NULL);
}
int DataBase::newGame()
{
	std::string q = "INSERT INTO t_games(status, start_time, end_time) VALUES(0,CURRENT_TIMESTAMP,null);";
	std::map<std::string, std::string> data;
	sqlite3_exec(db, q.c_str(), NULL, NULL, NULL);

	q = "SELECT last_insert_rowid() FROM t_games;";
	data["last_insert_rowid()"];
	sqlite3_exec(db, q.c_str(), getValuesMap, &data, NULL);
	return std::stoi(data["last_insert_rowid()"]);
}
void DataBase::endGame(int gameId)
{
	std::string q = "UPDATE t_games SET status=1, end_time=CURRENT_TIMESTAMP WHERE game_id=";
	q += std::to_string(gameId) + ";";
	sqlite3_exec(db, q.c_str(), NULL, NULL, NULL);
}
void DataBase::answer(int gameId, const std::string& name, int qId, const std::string& a, int correct, int time)
{
	std::string q = "INSERT INTO t_players_answers(game_id, username, question_id, player_answer, is_correct, answer_time) VALUES (";
	q += std::to_string(gameId) + ", ";
	q += "\"" + name + "\", ";
	q += std::to_string(qId) + ", ";
	q += "\"" + a + "\", ";
	q += std::to_string(correct) + ", ";
	q += std::to_string(time) + ");";
	sqlite3_exec(db, q.c_str(), NULL, NULL, NULL);
}
int getScores(void* d, int n, char** values, char** columns)
{
	auto v = (std::vector<std::pair<std::string, int>>*)d;
	std::pair<std::string, int> p;
	if (columns[0][0] == 'u')
		p = std::pair<std::string, int>(values[0], std::stoi(values[1]));
	else
		p = std::pair<std::string, int>(values[1], std::stoi(values[0]));
	v->push_back(p);
	return 0;
}
std::vector<std::pair<std::string, int>> DataBase::getHighScores()
{
	std::string q = "Select username, COUNT(is_correct) FROM t_players_answers WHERE is_correct=1 GROUP BY username ORDER BY -COUNT(is_correct) LIMIT 3;";
	std::vector<std::pair<std::string, int>> v;
	sqlite3_exec(db, q.c_str(), getScores, &v, NULL);
	return v;
}
std::string DataBase::getPersonalStatus(const std::string& name)
{
	std::string ret = "";
	std::map<std::string, std::string> values;
	values["games"] = values["correct"] = values["wrong"] = values["aTime"] = "0";
	float t;

	std::string q = "SELECT COUNT(DISTINCT game_id) as games FROM t_players_answers WHERE username=\"" + name + "\";";
	sqlite3_exec(db, q.c_str(), getValuesMap, &values, NULL);

	q = "SELECT COUNT(is_correct) as correct FROM t_players_answers WHERE username = \"" + name + "\" AND is_correct = \"1\"";
	sqlite3_exec(db, q.c_str(), getValuesMap, &values, NULL);

	q = "SELECT COUNT(is_correct) as wrong FROM t_players_answers WHERE username = \"" + name + "\" AND is_correct = \"0\"";
	sqlite3_exec(db, q.c_str(), getValuesMap, &values, NULL);

	q = "SELECT AVG(answer_time) as aTime FROM t_players_answers WHERE username=\"" + name + "\";";
	sqlite3_exec(db, q.c_str(), getValuesMap, &values, NULL);
	t = stof(values["aTime"]);

	ret  = Helper::getPaddedNumber(std::stoi(values["games"]), 4);
	ret += Helper::getPaddedNumber(std::stoi(values["correct"]), 6);
	ret += Helper::getPaddedNumber(std::stoi(values["wrong"]), 6);
	ret += Helper::getPaddedNumber((int)t, 2);
	ret += Helper::getPaddedNumber((int)((t-(int)t)*100), 2);
	return ret;
}
