#include "Server.h"

void Server::logout(std::vector<std::string> cmd)
{
	std::string nickName = cmd[0];
	for (auto it = client.begin(); it != client.end(); it++)
	{ 
		if (it->first == nickName)
		{
			it->second.closeClient();
			client.erase(it);
			return;
		}
	}
}

void Server::login(std::vector<std::string> cmd)
{
	std::string login = cmd[0];
	std::string password = cmd[1];

	auto result = this->dataBase->checkLogin(login);
	if (login == result["name"] && password == result["password"])
		client.push_back(std::make_pair(login, Client(cmd[2], result["name"])));
}