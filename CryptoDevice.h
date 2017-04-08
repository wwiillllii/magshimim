#pragma once

#include <cstdio>
#include <iostream>
#include "osrng.h"
#include "modes.h"
#include <string.h>
#include <cstdlib>

#include <map>

#include "integer.h"
using namespace std;


class CryptoDevice
{

public:
	CryptoDevice();
	~CryptoDevice();
	string encryptAES(string);
	string decryptAES(string);

	string md5(string);
};
