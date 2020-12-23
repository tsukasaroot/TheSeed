#include "npcSpawner.h"

npcSpawner::npcSpawner()
{
	this->reader = new xmlParser("npcs.xml");
	for (auto it = this->reader->Data.begin(); it != this->reader->Data.end(); it++)
	{
		auto parent = it->first;
		auto functions = this->reader->cleanData(parent);
		npcs.insert(std::pair<std::string, std::vector<std::string>>(parent, functions));
	}
	std::cout << "npcSpawner initialized: " << this->npcs.size() << " npc(s) loaded" << std::endl;
}