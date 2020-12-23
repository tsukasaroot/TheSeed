#include "inventoryManager.h"
#include "npcSpawner.h"

std::map<std::string, std::vector<std::string>> stockXML(xmlParser *reader)
{
	std::map<std::string, std::vector<std::string>> object;

	for (auto it = reader->Data.begin(); it != reader->Data.end(); it++)
	{
		auto parent = it->first;
		auto functions = reader->cleanData(parent);
		object.insert(std::pair<std::string, std::vector<std::string>>(parent, functions));
	}
	return object;
}