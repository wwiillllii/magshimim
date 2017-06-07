#include <iostream>

#include "RecievedMessage.h"
#include "Helper.h"

#define RM RecievedMessage

//static const void((*handlerFuncs[])(SOCKET));

void((RM::*handlerFuncs[])(SOCKET)) = {
	&RM::handle200,	&RM::handle201,	NULL,			&RM::handle203,
	NULL,			&RM::handle205,	NULL,			&RM::handle207,
	NULL,			&RM::handle209,	NULL,			&RM::handle211,
	NULL,			&RM::handle213,	NULL,			&RM::handle215,
	NULL,			&RM::handle217,	NULL,			&RM::handle219,
	NULL,			NULL,			&RM::handle222,	&RM::handle223,
	NULL,			&RM::handle225};

RM::RecievedMessage()
{
	socket = INVALID_SOCKET;
	user = NULL;
	code = -1;
}

RM::RecievedMessage(SOCKET s)
{
	this->code = Helper::getMessageTypeCode(s);
	this->socket = s;
	if (code / 100 == 2)
	{
		unsigned int index = code % 100;
		if (index == 99)
		{
			handle299(s);
			return;
		}
		else if (index < 26 && handlerFuncs[index])
		{
			(this->*handlerFuncs[index])(s);
			return;
		}
	}
	std::cerr << "Illigal type code for a packet from user " << /*user*/ "" << " : " << this->code << std::endl;
}
RM::~RecievedMessage(){}
RecievedMessage& RM::operator = (const RecievedMessage& other)
{
	socket = other.socket;
	user = other.user;
	code = other.code;
	values = other.values;
	return *this;
}

unsigned int RM::getCode() const { return code; }
User* RM::getUser() const { return user; }
void RM::setUser(User* user){ this->user = user; }
SOCKET RM::getSocket() const { return socket; }
std::string RM::operator[](unsigned int i) const { return values[i]; }// May throw out_of_range exception

std::string getStringWithLen(SOCKET s, int lenBytes)
{
	return Helper::getStringPartFromSocket(s, Helper::getIntPartFromSocket(s, lenBytes));
}

void RM::handle200(SOCKET s)
{
	values.push_back(getStringWithLen(s, 2));	// Username
	values.push_back(getStringWithLen(s, 2));	// Password
}
void RM::handle201(SOCKET s){ return; }
void RM::handle203(SOCKET s)
{
	values.push_back(getStringWithLen(s, 2));	// Username
	values.push_back(getStringWithLen(s, 2));	// Password
	values.push_back(getStringWithLen(s, 2));	// Mail
}
void RM::handle205(SOCKET s){ return; }
void RM::handle207(SOCKET s)
{
	values.push_back(Helper::getStringPartFromSocket(s,4));	// RoomId
}
void RM::handle209(SOCKET s)
{
	values.push_back(Helper::getStringPartFromSocket(s, 4));	// RoomId
}
void RM::handle211(SOCKET s){ return; }
void RM::handle213(SOCKET s)
{
	values.push_back(getStringWithLen(s, 2));					// RoomName
	values.push_back(Helper::getStringPartFromSocket(s, 1));	// NumPlayers
	values.push_back(Helper::getStringPartFromSocket(s, 2));	// NumQuestions
	values.push_back(Helper::getStringPartFromSocket(s, 2));	// SecondsPerQuestion
}
void RM::handle215(SOCKET s){ return; }
void RM::handle217(SOCKET s){ return; }
void RM::handle219(SOCKET s)
{
	values.push_back(Helper::getStringPartFromSocket(s, 1));	// AnswerNum
	values.push_back(Helper::getStringPartFromSocket(s, 2));	// AnswerTime
}
void RM::handle222(SOCKET s){ return; }
void RM::handle223(SOCKET s){ return; }
void RM::handle225(SOCKET s){ return; }
void RM::handle299(SOCKET s){ return; }