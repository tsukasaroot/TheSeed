#include "server.h"

void Server::sendAllClientsName(std::map<std::string, std::string> cmd)
{
	if (checkAll(3, cmd, &this->playerList))
	{
		std::string currentClient = cmd["id"];
		std::string listName = "C_GETALLCLIENTSNAME";

		for (auto it = this->_client.begin(); it != this->_client.end(); it++)
		{
			listName = listName + ':' + it->second->getNickName();
		}
		this->_client[currentClient]->clientWrite(listName);
	}
}

void Server::sendClientData(std::map<std::string, std::string> cmd)
{
	if (checkAll(3, cmd, &this->playerList))
	{
		std::string id = cmd["id"];

		auto dataToSend = "C_GETCLIENTDATA:" + this->_client[id]->getAll();
		this->_client[id]->clientWrite(dataToSend);
	}
}

void Server::sendProfile(std::map<std::string, std::string> cmd)
{
	if (checkAll(3, cmd, &this->playerList))
	{
		std::string id = cmd["id"];

		auto dataToSend = "C_GETPROFILE:" + this->_client[id]->getNickName() + ':' + this->_client[id]->getAll();
		this->_client[id]->clientWrite(dataToSend);
	}
}

void Server::getPosition(std::map<std::string, std::string> cmd)
{
	if (checkAll(6, cmd, &this->playerList))
	{
		std::string id = cmd["id"];

		this->_client[id]->setPositionQuery(cmd);
		if (this->_client[id]->getAbnormalities() > this->abnormalitiesTolerance)
		{
			std::map<std::string, std::string> data;
			data.insert(std::pair<std::string, std::string>("id", id));
			data.insert(std::pair<std::string, std::string>("ip", cmd["ip"]));
			client_exit(data);
		}
	}
}