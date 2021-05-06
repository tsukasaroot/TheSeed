#include "Server.h"

std::string decipherPacket(std::string toDecipher, std::string salt)
{
	std::vector<std::string> toprocess;
	size_t pos = 0;
	std::string unciphered = "";
	std::string binariedSalt = "";
	std::string delimiter = "A";

	for (char& _char : salt)
		binariedSalt += std::bitset<8>(_char).to_string();

	while ((pos = toDecipher.find(delimiter)) != std::string::npos)
	{
		std::string token = toDecipher.substr(0, toDecipher.find(delimiter));

		if (!token.empty())
		{
			std::cout << toDecipher.size() << std::endl;
			toprocess.push_back(token);
		}
		toDecipher.erase(0, pos + delimiter.size());
		token.clear();
	}

	for (auto it = toDecipher.begin(); it != toDecipher.end(); it++)
	{
		std::cout << "test" << '\n';
	}
}

std::string cipherPacket(std::string toCipher, std::string salt)
{
	std::string ciphered = "";
	std::string binariedSalt = "";

	for (char& _char : salt)
		binariedSalt += std::bitset<8>(_char).to_string();

	for (char& _char : toCipher)
	{
		if (_char == '\0')
		{
			ciphered += _char;
			break;
		}
		ciphered += std::to_string(_char * binariedSalt.size()) + 'A';
	}
	decipherPacket(ciphered, salt);

	return ciphered;
}

std::map<std::string, std::vector<std::string>> stockXML(xmlParser* reader)
{
	std::map<std::string, std::vector<std::string>> object;

	for (auto it = reader->Data.begin(); it != reader->Data.end(); it++)
	{
		auto parent = reader->cleanParent(it->first);
		auto functions = reader->cleanData(it->second);

		object.insert(std::pair<std::string, std::vector<std::string>>(parent, functions));
	}
	return object;
}

bool checkAll(int size, std::vector<std::string> cmd, std::vector<std::string>* playerList)
{
	if (cmd.size() == size)
	{
		std::string nickName = cmd[0];
		auto it = std::find(playerList->begin(), playerList->end(), nickName);
		if (it != playerList->end())
			return true;
		else
			std::cerr << "Error: " << nickName << " this player don't exist, command denied." << std::endl;
	}
	return false;
}

std::string packetBuilder(std::vector<std::string> requests)
{
	std::string result;

	for (auto it = requests.begin(); it != requests.end(); it++)
	{
		result += *it;
		if (it + 1 != requests.end())
			result += ':';
	}
	return result;
}