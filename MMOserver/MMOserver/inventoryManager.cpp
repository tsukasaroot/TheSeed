#include "inventoryManager.h"

inventoryManager::inventoryManager()
{
	this->reader = new xmlParser("items.xml");
	
	for (auto it = this->reader->Data.begin(); it != this->reader->Data.end(); it++)
	{
		auto parent = it->first;
		auto functions = this->reader->cleanData(parent);
		items.insert(std::pair<std::string, std::vector<std::string>>(parent, functions));
	}
	std::cout << "inventoryManager initialized: " << this->items.size() << " item(s) loaded" << std::endl;
}