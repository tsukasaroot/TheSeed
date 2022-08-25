#include "Server.h"

std::map<std::string, std::string> formatStringAssociative(std::string line)
{
	std::map<std::string, std::string> buildedCommands;
	std::string token;

	while (line.find(':') != std::string::npos)
	{
		token = line.substr(0, line.find(':'));
		line.erase(0, token.length() + 1);

		auto key = token.substr(0, token.find('{'));
		token.erase(0, key.length() + 1);

		auto value = token.substr(0, token.find('}'));
		token.erase(0, value.length() + 1);

		buildedCommands.insert(std::pair<std::string, std::string>(key, value));
		token.clear();
	}

	return buildedCommands;
}

std::vector<char> openXml(const char* path)
{
	if (!std::filesystem::exists(path))
	{
		std::cerr << "File " << path << " not found" << std::endl;
		exit(1);
	}
	std::cout << "Loading file " << path << std::endl;
	std::ifstream theFile(path);
	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	
	buffer.push_back('\0');
	theFile.close();

	return buffer;
}

std::string decipherPacket(std::string toDecipher, std::string salt)
{
	size_t pos = 0;
	std::string unciphered = "";
	std::string delimiter = "A";


	while ((pos = toDecipher.find(delimiter)) != std::string::npos)
	{
		std::string token = toDecipher.substr(0, toDecipher.find(delimiter));

		if (!token.empty())
		{
			int i = std::stoi(token);
			i = i / salt.size();
			unciphered += i;
		}
		toDecipher.erase(0, pos + delimiter.size());
		token.clear();
	}
	return unciphered;
}

std::string cipherPacket(std::string toCipher, std::string salt)
{
	std::string ciphered = "";

	for (char& _char : toCipher)
	{
		ciphered += std::to_string(_char * salt.size()) + 'A';
	}
	return toCipher;
}

std::string generateSalt(std::string salt)
{
	std::string binariedSalt;
	
	for (char& _char : salt)
		binariedSalt += std::bitset<8>(_char).to_string();

	return binariedSalt;
}

bool checkAll(int size, std::map<std::string, std::string> cmd, std::vector<std::string>* playerList)
{
	if (cmd.size() == size)
	{
		std::string nickName = cmd["id"];
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