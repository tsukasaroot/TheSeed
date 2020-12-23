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
	xmlParser* reader;
};

#endif