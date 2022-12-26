#include "inventoryManager.h"

inventoryManager::inventoryManager(SQLManager *db)
{
	this->dataBase = db;
}

void inventoryManager::addItem(std::vector<int> item)
{

}

void inventoryManager::getClientInventory(int client)
{
	std::vector<std::string> fields;
	std::vector<std::string> columnName;
	std::vector<std::string> where;

	fields.push_back("items_id");
	fields.push_back("player_id");
	columnName.push_back("items_id");
	columnName.push_back("player_id");
	where.push_back("1");
	where.push_back("1");

	this->inventory = this->dataBase->getInventory("inventory", fields, columnName, where);
	std::cout << "inventoryManager initialized for player: " << std::endl;
}

void inventoryManager::saveClientInventory(std::string cmd)
{

}