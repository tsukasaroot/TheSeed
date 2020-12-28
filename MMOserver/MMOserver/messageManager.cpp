#include "messageManager.h"

messageManager::messageManager()
{

}

void  messageManager::sendPrivateMessage(Client* clientFrom, Client* clientToSend, std::string message)
{
	int timestamp = 27122020;
	std::string toSend = clientFrom->getNickName() + ':' + message + ':' + std::to_string(timestamp);

	clientToSend->clientWrite(toSend);
}

void messageManager::sendGlobalMessage(Client* clientFrom, std::string message, std::map<std::string, Client*> clientsToSend)
{
	int timestamp = 27122020;
	std::string toSend = clientFrom->getNickName() + ':' + message + ':' + std::to_string(timestamp);

	for (auto it = clientsToSend.begin(); it != clientsToSend.end(); it++)
	{
		if (it->first != clientFrom->getNickName())
			it->second->clientWrite(toSend);
	}
}
