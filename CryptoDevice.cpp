#include "CryptoDevice.h"

#include "hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include "aes.h"
using CryptoPP::AES;

#include "ccm.h"
using CryptoPP::CBC_Mode;

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <md5.h>

using CryptoPP::HashFilter;

//aquire a default key for AES and block size for CBC. can't generate randomly, since all the agents shuold have the same key.
//can initialize with a unique sequence but not needed.
byte key[CryptoPP::AES::DEFAULT_KEYLENGTH], iv[CryptoPP::AES::BLOCKSIZE];


CryptoDevice::CryptoDevice(){}
CryptoDevice::~CryptoDevice(){}


string CryptoDevice::encryptAES(string plainText)
{

	string cipherText;
	//cout << endl << "Plain text: " << plainText << endl;
	CBC_Mode< AES >::Encryption e;
	e.SetKeyWithIV(key, sizeof(key), iv);

	StringSource s(plainText, true,
		new StreamTransformationFilter(e, new HexEncoder(new StringSink(cipherText))));

	//cout << "Cipher text: " << cipherText << endl << endl;
	return cipherText;
}

string CryptoDevice::decryptAES(string cipherText)
{

	string decryptedText;
	//cout << endl << "Cipher text: " << cipherText << endl;
	
	CBC_Mode< AES >::Decryption d;
	d.SetKeyWithIV(key, sizeof(key), iv);

	StringSource s(cipherText, true,
		new HexDecoder(new StreamTransformationFilter(d, new StringSink(decryptedText))));

	//cout << "Decrypted text: " << decryptedText << endl << endl;
	return decryptedText;
}

string CryptoDevice::md5(string in)
{
	string out;

	CryptoPP::Weak::MD5 hash;
	StringSource s(in, true, new HashFilter(hash, new HexEncoder(new StringSink(out))));

	return out;
}