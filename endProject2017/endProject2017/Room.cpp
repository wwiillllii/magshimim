#include <algorithm>
#include <iostream>
#include <thread>

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
	this->_isGame = false;
	this->questionIndex = 0;
	admin->setRoom(this);
	users[admin] = std::pair<bool, int>(false, 0);
}

void Room::updateUsers()
{
	auto it = users.begin();
	const string msg = "108" + Helper::getPaddedNumber(users.size(),1) + getUserNames();
	while (it != users.end())
		Helper::sendData((it++)->first->getSocket(), msg);
}
void Room::addUser(User* user)
{
	user->setRoom(this);
	users[user] = std::pair<bool, int>(false, 0);
	updateUsers();
}
void Room::removeUser(User* user)
{
	users.erase(user);
	user->setRoom(NULL);
	if (user == admin)
		close();
	else
		updateUsers();
}
void Room::close()
{
	auto it = users.begin();
	std::string msg;
	if (_isGame)
	{
		msg = "121";
		msg += ('0' + users.size());
		while (it != users.end())
		{
			const User* u = it->first;
			msg += Helper::getPaddedNumber(u->getName().size(), 2);
			msg += u->getName();
			msg += Helper::getPaddedNumber((it++)->second.second, 2);
		}
	}
	else msg = "116";
	it = users.begin();
	while (it != users.end())
	{
		Helper::sendData(it->first->getSocket(), msg);
		it->first->setRoom(NULL);
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
	auto it = users.begin();
	while (it != users.end())
	{
		ret += Helper::getPaddedNumber(it->first->getName().size(),2);
		ret += it->first->getName();
		it++;
	}
	return ret;
}

bool Room::isGame()
{
	return _isGame;
}
void Room::startGame(DataBase& db)
{
	_isGame = true;
	db.generateQuestions(&questions, numQuestions);
}
bool Room::nextQuestion()
{
	if (questionIndex >= numQuestions) return false;
	std::string msg = "118";
	std::string str;
	Question& q = questions[questionIndex++];
	auto it = users.begin();
	int i;

	str = q.getQuestion();
	msg += Helper::getPaddedNumber(str.size(), 3);
	msg += str;

	for (i = 0; i < 4; i++)
	{
		str = q.getAnswer(i);
		msg += Helper::getPaddedNumber(str.size(), 3);
		msg += str;
	}

	for (; it != users.end(); it++)
		Helper::sendData(it->first->getSocket(), msg);
	return true;
}
void Room::answer(User* u, int a, int t)
{
	std::string msg = "120";
	int correct = (t <= secondsPerQuestion && a == questions[questionIndex-1].correctAnswer()) ? 1 : 0;
	msg += (correct + '0');
	users[u].first = true;
	users[u].second += correct;
	Helper::sendData(u->getSocket(), msg);
}