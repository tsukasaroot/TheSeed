#ifndef INVENTORYMANAGER_H_
#define INVENTORYMANAGER_H_

#include "SQLmanager.h"

#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <map>
#include <vector>

class inventoryManager {
public:
	inventoryManager(SQLManager* db);
	void addItem(std::vector<int> item);
	void saveClientInventory(std::string cmd);
	void getClientInventory(int client);
private:
	std::string inventory;
	SQLManager* dataBase;
	xml_node<char>* items;
};

#endif