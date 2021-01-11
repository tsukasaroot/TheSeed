#include "server.h"

void Server::sendAllClientsName(std::vector<std::string> cmd)
{
	if (cmd.size() == 2)
	{
		std::string currentClient = cmd[0];
		std::string listName = "C_GETALLCLIENTSNAME";

		for (auto it = this->_client.begin(); it != this->_client.end(); it++)
		{
			listName = listName + ':' + it->second->getNickName();
		}
		this->_client[currentClient]->clientWrite(listName);
	}
}

void Server::sendClientData(std::vector<std::string> cmd)
{
	if (cmd.size() == 2)
	{
		std::string nickName = cmd[0];
		auto it = std::find(this->playerList.begin(), this->playerList.end(), nickName);

		if (it != this->playerList.end())
		{
			auto dataToSend = "C_GETCLIENTDATA:" + this->_client[nickName]->getAll();
			this->_client[nickName]->clientWrite(dataToSend);
			return;
		}
		std::cerr << "Error: " << nickName << " this player don't exist, command denied." << std::endl;
		return;
	}
}

void Server::sendProfile(std::vector<std::string> cmd)
{
	if (cmd.size() == 2)
	{
		std::string nickName = cmd[0];
		auto it = std::find(this->playerList.begin(), this->playerList.end(), nickName);
		if (it != this->playerList.end())
		{
			auto dataToSend = "C_GETPROFILE:" + this->_client[nickName]->getNickName() + ':' + this->_client[nickName]->getAll();
			this->_client[nickName]->clientWrite(dataToSend);
			return;
		}
		std::cerr << "Error: " << nickName << " this player don't exist, command denied." << std::endl;
		return;
	}
}

void Server::getPosition(std::vector<std::string> cmd)
{
	if (cmd.size() == 5)
	{
		std::cout << "position received" << std::endl;
	}
}