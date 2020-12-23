#include "inventoryManager.h"

inventoryManager::inventoryManager()
{
	this->reader = new xmlParser("items.xml");
	
	items = stockXML(this->reader);
	std::cout << "inventoryManager initialized: " << this->items.size() << " item(s) loaded" << std::endl;
}

void inventoryManager::addItem(std::vector<int> item)
{

}

std::vector<int> inventoryManager::getClientInventory(std::string client)
{

}