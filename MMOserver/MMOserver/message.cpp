#include "server.h"

void Server::privateMessage(std::vector<std::string> cmd)
{
	if (cmd.size() == 4)
	{
		std::string nickName = cmd[0];
		auto it = std::find(this->playerList.begin(), this->playerList.end(), nickName);
		
		if (it != this->playerList.end())
		{
			Client* sender = this->_client[cmd[0]];
			Client* receiver = this->_client[cmd[1]];

			this->messages->sendPrivateMessage(sender, receiver, cmd[2]);
		}
	}
}

void Server::globalMessage(std::vector<std::string> cmd)
{
	if (cmd.size() == 3)
	{
		std::string nickName = cmd[0];
		auto it = std::find(this->playerList.begin(), this->playerList.end(), nickName);
		if (it != this->playerList.end())
		{
			Client* sender = this->_client[cmd[0]];

			this->messages->sendGlobalMessage(sender, cmd[1], this->_client);
		}
	}
}