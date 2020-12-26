#ifndef MESSAGEMANAGER_H_
#define MESSAGEMANAGER_H_

#include "xmlParser.h"

#include <windows.h>

#include <iostream>
#include <string>
#include <thread>
#include <map>
#include <vector>


class messageManager {
public:
	messageManager();
private:
	xmlParser* reader;
	std::map<std::string, std::vector<std::string>> messages;
	std::map<std::string, int> timestamp;
};

#endif