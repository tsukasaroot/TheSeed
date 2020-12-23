#include "npcSpawner.h"

npcSpawner::npcSpawner()
{
	this->reader = new xmlParser("npcs.xml");
	auto functions = this->reader->cleanData(reader->Data["<npcs>"]);
	std::cout << "npcSpawner initialized " << functions.size() << " npcs loaded" << std::endl;
}