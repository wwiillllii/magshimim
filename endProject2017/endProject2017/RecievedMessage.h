#include <WinSock2.h>
#include <vector>

#include "User.h"

class RecievedMessage
{
public:
	RecievedMessage();
	RecievedMessage(SOCKET s);
	~RecievedMessage();
	RecievedMessage& operator = (const RecievedMessage& other);

	unsigned int getCode() const;
	User* getUser() const;
	void setUser(User* user);
	SOCKET getSocket() const;
	std::string operator[](unsigned int i) const;
	std::string toString();

	void handle200(SOCKET s);
	void handle201(SOCKET s);
	void handle203(SOCKET s);
	void handle205(SOCKET s);
	void handle207(SOCKET s);
	void handle209(SOCKET s);
	void handle211(SOCKET s);
	void handle213(SOCKET s);
	void handle215(SOCKET s);
	void handle217(SOCKET s);
	void handle219(SOCKET s);
	void handle222(SOCKET s);
	void handle223(SOCKET s);
	void handle225(SOCKET s);
	void handle299(SOCKET s);

private:
	unsigned int code;
	SOCKET socket;
	User* user;
	std::vector<std::string> values;
};