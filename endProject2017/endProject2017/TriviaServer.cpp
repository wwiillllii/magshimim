#include "TriviaServer.h"
#include "Helper.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>

#define TS TriviaServer

std::queue<RecievedMessage> messages;
std::mutex messagesMTX;
std::condition_variable messagesCV;
bool running = true;

void(TS::*messageHandlers[])(RecievedMessage&){
	&TS::handle200, &TS::handle201, NULL,			&TS::handle203,
	NULL,			&TS::handle205, NULL,			&TS::handle207,
	NULL,			&TS::handle209, NULL,			&TS::handle211,
	NULL,			&TS::handle213, NULL,			&TS::handle215,
	NULL,			&TS::handle217, NULL,			&TS::handle219,
	NULL,			NULL,			&TS::handle222, &TS::handle223,
	NULL,			&TS::handle225};

TS::TriviaServer()
{
	db = DataBase();
	socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (socket == INVALID_SOCKET)
		std::cerr << WSAGetLastError() << std::endl;
}

TS::~TriviaServer()
{
	connectedUsers.clear();
	rooms.clear();
	closesocket(socket);
}

void TS::messageHandler()
{
	RecievedMessage msg;
	const auto t = std::chrono::milliseconds(500);
	
	while (running)
	{
		std::unique_lock<std::mutex> lock(messagesMTX);
		if (!messagesCV.wait_for(lock, t, []{ return !messages.empty(); }))
		{
			lock.unlock();
			continue;
		}
		msg = messages.front();
		messages.pop();
		lock.unlock();
		msg.setUser(getUserBySocket(msg.getSocket()));
		unsigned int code = msg.getCode();
		if (code / 100 == 2)
		{
			unsigned int index = code % 100;
			if (index == 99) handle299(msg);
			else if (index < 26 && messageHandlers[index])
				(this->*messageHandlers[index])(msg);
			else
				std::cerr << "Unknown type code: " << code << std::endl;
		}else
			std::cerr << "Illigal type code: " << code << std::endl;
	}
}

// User signin
void TS::handle200(RecievedMessage& msg)
{
	if (!db.signin(msg[0],msg[1]))
	{
		// Wrong details
		Helper::sendData(msg.getSocket(), "1021");
	}
	else if (connectedUsers.end() != 
		std::find_if(
			connectedUsers.begin(),
			connectedUsers.end(),
			[msg](std::pair<SOCKET, User*> p)
	{ return p.second ? p.second->getName() == msg[0] : false; }
			)
		)
	{
		// User is already connected
		Helper::sendData(msg.getSocket(), "1022");
	}
	else
	{
		// Success
		connectedUsers[msg.getSocket()] = new User(msg[0], msg.getSocket());
		Helper::sendData(msg.getSocket(), "1020");
	}
}
// User signout
void TS::handle201(RecievedMessage& msg){}
// User signup
void TS::handle203(RecievedMessage& msg)
{
	Helper::sendData(msg.getSocket(),
		"104" + Helper::getPaddedNumber(
		db.signup(msg[0], msg[1], msg[2]), 1));
}
// Get room list
void TS::handle205(RecievedMessage& msg)
{
	std::string ret = "";
	int count = 0;
	std::map<int, Room*>::iterator it = rooms.begin();
	while (it != rooms.end())
	{
		if (!it->second->isGame())
		{
			ret += Helper::getPaddedNumber(it->first, 4);
			ret += Helper::getPaddedNumber(it->second->getName().size(), 2);
			ret += it->second->getName();
			count++;
		}
		it++;
	}
	ret = "106" + Helper::getPaddedNumber(count,4) + ret;
	Helper::sendData(msg.getSocket(), ret);
}
// Get users connected to room
void TS::handle207(RecievedMessage& msg)
{
	std::string ret = "108";
	Room* r = rooms[stoi(msg[0])];
	ret += Helper::getPaddedNumber(r->getUsersCount(), 1);
	ret += r->getUserNames();
	Helper::sendData(msg.getSocket(), ret);
}
// Connect to room
void TS::handle209(RecievedMessage& msg)
{
	std::string ret = "110";
	Room* r = NULL;
	try{
		r = rooms.at(stoi(msg[0]));
	}
	catch (out_of_range&){
			ret += "2";
			Helper::sendData(msg.getSocket(), ret);
			return;
	}
	if (r->getUsersCount() >= r->getMaxUsers())
	{
		ret += "1";
		Helper::sendData(msg.getSocket(), ret);
		return;
	}
	ret += "0";
	ret += Helper::getPaddedNumber(r->getNumQuestions() , 2);
	ret += Helper::getPaddedNumber(r->getSecondsPerQuestion(), 2);
	Helper::sendData(msg.getSocket(), ret); 
	r->addUser(msg.getUser());
}
// Leave room
void TS::handle211(RecievedMessage& msg)
{
	User* u = msg.getUser();
	if (u == NULL) return;
	Room* r = u->getRoom();
	if (r == NULL) return;
	r->removeUser(u);
	Helper::sendData(msg.getSocket(), "1120");
}
int TS::nextRoomId()
{
	int ret = 0;
	map<int, Room*>::iterator it = rooms.begin();
	while (it != rooms.end())
	{
		if (it->first == ret)
		{
			ret++;
			it = rooms.begin();
		}
		else it++;
	}
	return ret;
}
// Create room
void TS::handle213(RecievedMessage& msg)
{
	User* u = msg.getUser();
	if (u == NULL) return;
	Room* r = NULL;
	try
	{
		r = new Room(u,
			msg[0],
			stoi(msg[1]),
			stoi(msg[2]),
			stoi(msg[3]),
			&db);
	}
	catch (exception&)
	{
		if (r) delete r;
		Helper::sendData(msg.getSocket(), "1141");
		return;
	}
	rooms[nextRoomId()] = r;
	Helper::sendData(msg.getSocket(), "1140");
	u->setRoom(r);
}
// Close room
void TS::handle215(RecievedMessage& msg)
{
	User* u = msg.getUser();
	if (u == NULL) return;
	Room* r = u->getRoom();
	if (r == NULL) return;
	r->close();
	rooms.erase(std::find_if(rooms.begin(), rooms.end(), [r](std::pair<int,Room*> p)
	{
		return p.second == r;
	}));
}
// Start game
void TS::handle217(RecievedMessage& msg)
{
	User* u = msg.getUser();
	if (u == NULL) return;
	Room* r = u->getRoom();
	if (r == NULL) return;
	r->startGame();
	std::thread(&TS::gameClock, this, r).detach();
}
// Player answer
void TS::handle219(RecievedMessage& msg)
{
	User* u = msg.getUser();
	if (u == NULL) return;
	Room* r = u->getRoom();
	if (r == NULL) return;
	r->answer(u, stoi(msg[0]), stoi(msg[1]));
}
// Player left game
void TS::handle222(RecievedMessage& msg)
{
	User* u = msg.getUser();
	if (u == NULL) return;
	Room* r = u->getRoom();
	if (r == NULL) return;
	r->removeUser(u);
}
// Best scores
void TS::handle223(RecievedMessage& msg)
{
	std::vector<std::pair<std::string, int>> scores = db.getHighScores();
	unsigned int i = 0;
	std::string ret = "124";
	for (i = 0; i < scores.size(); i++)
	{
		ret += Helper::getPaddedNumber(scores[i].first.size(), 2);
		ret += scores[i].first;
		ret += Helper::getPaddedNumber(scores[i].second, 6);
	}
	for (i; i < 3; i++)
		ret += "00"   "000000";
	Helper::sendData(msg.getSocket(), ret);
}
// Personal status
void TS::handle225(RecievedMessage& msg)
{
	Helper::sendData(msg.getSocket(), "126" + db.getPersonalStatus(msg.getUser()->getName()));
}
// User disconnected
void TS::handle299(RecievedMessage& msg)
{
	User* u = msg.getUser();
	if (u)
	{
		Room* r = u->getRoom();
		if (r)
		{
			r->removeUser(u);
			if (!r->getUsersCount())
				rooms.erase(std::find_if(rooms.begin(), rooms.end(), [r](std::pair<int, Room*> p)
				{
					return p.second == r;
				}));
		}
	}
	closesocket(msg.getSocket());
	connectedUsers.erase(msg.getSocket());
}

void TS::bindAndListen()
{
	try
	{
		struct sockaddr_in sa = { 0 };
		sa.sin_port = htons(PORT);
		sa.sin_family = AF_INET;
		sa.sin_addr.s_addr = INADDR_ANY;
		if (::bind(socket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
			throw std::exception(__FUNCTION__ " - bind");

		if (::listen(socket, SOMAXCONN) == SOCKET_ERROR)
			throw std::exception(__FUNCTION__ " - listen");

		std::thread(&TS::messageHandler, this).detach();
	}
	catch (std::exception e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << "Last error: " << WSAGetLastError() << std::endl;
		system("pause");
	}
}
void TS::gameClock(Room* r)
{
	while (r->nextQuestion())
		std::this_thread::sleep_for(std::chrono::seconds(r->getSecondsPerQuestion()));
	rooms.erase(std::find_if(rooms.begin(), rooms.end(), [r](std::pair<int, Room*> p)
	{
		return p.second == r;
	}));
	r->close();
}
User* TS::getUserBySocket(SOCKET socket)
{
	User* user = NULL;
	try
	{
		user = connectedUsers.at(socket);
	}
	catch (out_of_range&){}
	return user;
}
void TS::clientHandler(SOCKET client)
{
	RecievedMessage msg;
	try
	{
		while (true)
		{
			{
				msg = RecievedMessage(client);
				std::lock_guard<std::mutex> lg(messagesMTX);
				messages.push(msg);
			}
			messagesCV.notify_one();
		}
	}
	catch (std::exception& e)
	{
		int error = WSAGetLastError(); 
		if (error != WSAENOTCONN)
			if (error = WSAECONNRESET)
				closesocket(client);
			else
				std::cerr << "Unexpected error in clientHandler for socket " << client << ":" << e.what() << std::endl;
	}
}
void TS::serve()
{
	fd_set fds;
	const struct timeval t{ 0, 250 };
	bindAndListen();
	while (running)
	{
		FD_ZERO(&fds);
		FD_SET(socket, &fds);
		if (select(0, &fds, NULL, NULL, &t))
			std::thread(&TS::clientHandler, this, ::accept(socket, NULL, NULL)).detach();
	}
}

int main()
{
	WSADATA wsa_data = {};
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
		throw std::exception("WSAStartup Failed");
	TS server;
	thread t(&TS::serve, &server);

	string input = "help";
	while (true)
	{
		if (input == "quit")
		{
			running = false;
			t.join();
			this_thread::sleep_for(std::chrono::milliseconds(1000));
			return 0;
		}
		else if (input == "help")
		{
			std::cout << "help - display this list." << std::endl;
			std::cout << "quit - close the server and exit" << std::endl;
		}
		else
			std::cout << "Unknown command! Type \"help\" for a list of commands." << std::endl;
		std::cin >> input;
	}
}