#include "Server.h"

void Server::Opcodesinitialize()
{
	this->reader = new xmlParser("initialiaze.xml");
	auto functions = this->reader->cleanData(reader->Data["<functions>"]);

	list.insert(std::pair<std::string, opcodes>("login", &Server::login));
	list.insert(std::pair<std::string, opcodes>("logout", &Server::logout));
	list.insert(std::pair<std::string, opcodes>("saveData", &Server::savePlayerData));
	list.insert(std::pair<std::string, opcodes>("getClientData", &Server::getClientData));

	std::cout << "OPCodes initialized: " << list.size() << " OPCodes loaded" << std::endl;

	this->inventory = new inventoryManager();
	this->npcSpawn = new npcSpawner();
	this->items = new itemsManager();
}

void Server::processOpcodes(std::vector<std::string> opcodes, std::string ip)
{
	for (auto& cmd : opcodes)
	{
		std::string token = cmd.substr(0, cmd.find(':'));
		cmd.erase(0, token.length() + 1);
		std::vector<std::string> args = formatString(cmd);

		(this->*list[token])(args);

		args.clear();
	}
}