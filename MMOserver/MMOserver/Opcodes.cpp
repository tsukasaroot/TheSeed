#include "Server.h"

void Server::Opcodesinitialize()
{
	this->reader = new xmlParser("initialiaze.xml");
	auto functions = this->reader->cleanData(reader->Data["<functions>"]);

	list.insert(std::pair<std::string, opcodes>("login", &Server::login));
	list.insert(std::pair<std::string, opcodes>("logout", &Server::logout));
	list.insert(std::pair<std::string, opcodes>("saveData", &Server::savePlayerData));

	this->inventory = new inventoryManager();
	this->npcSpawn = new npcSpawner();
}