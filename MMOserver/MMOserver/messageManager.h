#ifndef MESSAGEMANAGER_H_
#define MESSAGEMANAGER_H_

#include "xmlParser.h"
#include "Client.h"

#include <windows.h>

#include <iostream>
#include <string>
#include <thread>
#include <map>
#include <vector>


class messageManager {
public:
	messageManager();
	void sendPrivateMessage(Client* clientFrom, Client* clientToSend, std::string message);
	void sendGlobalMessage(Client* clientFrom, std::string message, std::map<std::string, Client*> clientsToSend);
private:
	xmlParser* reader;
	std::map<std::string, std::vector<std::string>> messages;
	std::map<std::string, int> timestamp;
	std::map<std::string, Client*> _client;
};

#endif