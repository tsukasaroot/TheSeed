#include "Server.h"

void Server::Opcodesinitialize()
{
	this->reader = new xmlParser("opcodes.xml");
	auto functions = this->reader->cleanData(reader->Data["<functions>"]);

	list.insert(std::pair<std::string, opcodes>("login", &Server::login));
	list.insert(std::pair<std::string, opcodes>("logout", &Server::logout));
	list.insert(std::pair<std::string, opcodes>("saveData", &Server::savePlayerData));
	list.insert(std::pair<std::string, opcodes>("getClientData", &Server::getClientData));
	list.insert(std::pair<std::string, opcodes>("privateMessage", &Server::privateMessage));
	list.insert(std::pair<std::string, opcodes>("globalMessage", &Server::globalMessage));
	list.insert(std::pair<std::string, opcodes>("getAllClientsName", &Server::getAllClientsName));

	std::cout << "OPCodes initialized: " << list.size() << " OPCodes loaded" << std::endl;

	this->items = new itemsManager();
	this->npcSpawn = new npcSpawner();
	this->messages = new messageManager();
	this->skills = new skillManager();
}

void Server::processOpcodes(std::vector<std::string> opcodes, std::string ip)
{
	for (auto& cmd : opcodes)
	{
		static int i = 0;
		std::string token = cmd.substr(0, cmd.find(':'));
		cmd.erase(0, token.length() + 1);
		std::vector<std::string> args = formatString(cmd);

		if (this->list.find(token) != this->list.end())
		{
			std::cout << i++ << std::endl;
			(this->*list[token])(args);
		}
		else
		{
			std::cerr << "Invalid opcode sent from: " << ip << std::endl;
		}
		args.clear();
	}
}