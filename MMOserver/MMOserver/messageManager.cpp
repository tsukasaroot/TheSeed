#include "messageManager.h"

messageManager::messageManager()
{

}

void  messageManager::sendPrivateMessage(Client* clientFrom, Client* clientToSend, std::string message)
{
	int date = 27122020;
	std::string mkdir = "mkdir messagesDumper\\" + clientFrom->getNickName();
	std::string writeFile = "echo \"" + message + "\" >> messagesDumper\\" + clientFrom->getNickName() + std::to_string(date);

	if (fs::exists("messagesDumper\\" + clientFrom->getNickName()) == false)
		std::system(mkdir.c_str());
	std::system(writeFile.c_str());
	std::string toSend = clientFrom->getNickName() + ':' + message + ':' + std::to_string(date);

	clientToSend->clientWrite(toSend);
}

void messageManager::sendGlobalMessage(Client* clientFrom, std::string message, std::map<std::string, Client*> clientsToSend)
{
	int date = 27122020;
	std::string toSend = clientFrom->getNickName() + ':' + message + ':' + std::to_string(date);

	for (auto it = clientsToSend.begin(); it != clientsToSend.end(); it++)
	{
		if (it->first != clientFrom->getNickName())
			it->second->clientWrite(toSend);
	}
}
