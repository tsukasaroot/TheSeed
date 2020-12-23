#include "inventoryManager.h"

inventoryManager::inventoryManager()
{
	this->reader = new xmlParser("items.xml");
	auto functions = this->reader->cleanData(reader->Data["<items>"]);
	std::cout << "inventoryManager initialized " << functions.size() << " items loaded" << std::endl;
}