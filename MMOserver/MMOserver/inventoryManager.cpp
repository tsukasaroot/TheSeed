#include "inventoryManager.h"

inventoryManager::inventoryManager()
{
	this->reader = new xmlParser("items.xml");
	std::cout << "inventoryManager initialized " << this->reader->Data.size() << " item(s) loaded" << std::endl;
}