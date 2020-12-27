#include "server.h"

void Server::getAllClientsName(std::vector<std::string> cmd)
{
	if (cmd.size() == 2)
	{
		std::string currentClient = cmd[0];
		std::string listName = "getAllClientsName";

		for (auto it = this->_client.begin(); it != this->_client.end(); it++)
		{
			listName = listName + ':' + it->second->getNickName();
		}
		this->_client[currentClient]->clientWrite(listName);
	}
}