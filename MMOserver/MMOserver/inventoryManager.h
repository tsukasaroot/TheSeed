#ifndef INVENTORYMANAGER_H_
#define INVENTORYMANAGER_H_

#include "xmlParser.h"

#include <windows.h>

#include <iostream>
#include <string>
#include <thread>
#include <map>

class inventoryManager {
public:
	inventoryManager();
private:
	xmlParser *reader;
	std::map<std::string, std::vector<std::string>> items;
};

#endif