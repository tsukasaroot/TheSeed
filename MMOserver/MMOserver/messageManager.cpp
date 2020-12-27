#include "messageManager.h"

messageManager::messageManager()
{

}

void  messageManager::sendMessage(Client* clientFrom, Client* clientToSend, std::string message)
{
	int timestamp = 27122020;
	std::string toSend = clientFrom->getNickName() + ':' + message + ':' + std::to_string(timestamp);

	clientToSend->clientWrite(toSend);
}