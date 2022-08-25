#include "Server.h"

void Server::privateMessage(std::map<std::string, std::string> cmd)
{
	if (checkAll(5, cmd, &this->playerList))
	{
		Client* sender = this->_client[cmd["id"]];
		Client* receiver = this->_client[cmd["receiver_id"]];

		this->messages->sendPrivateMessage(sender, receiver, "message{" + cmd["message"] + "}");
	}
}

void Server::globalMessage(std::map<std::string, std::string> cmd)
{
	if (checkAll(4, cmd, &this->playerList))
	{
		std::string player_id = cmd["id"];
		Client* sender = this->_client[player_id];

		this->messages->sendGlobalMessage(sender, "message{" + cmd["message"] + "}", this->_client);
	}
}