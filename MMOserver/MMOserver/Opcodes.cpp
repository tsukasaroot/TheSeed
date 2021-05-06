#include "Server.h"

void Server::Opcodesinitialize()
{
	this->reader = new xmlParser("opcodes.xml");
	auto functions = this->reader->cleanData(reader->Data["<functions>"]);
	std::string name = "&Server::login";

	list.insert(std::pair<std::string, opcodes>("S_LOGIN", &Server::login));
	list.insert(std::pair<std::string, opcodes>("S_LOGOUT", &Server::logout));
	list.insert(std::pair<std::string, opcodes>("S_SAVEDATA", &Server::savePlayerData));
	list.insert(std::pair<std::string, opcodes>("S_SENDCLIENTDATA", &Server::sendClientData));
	list.insert(std::pair<std::string, opcodes>("S_PRIVATEMESSAGE", &Server::privateMessage));
	list.insert(std::pair<std::string, opcodes>("S_GLOBALMESSAGE", &Server::globalMessage));
	list.insert(std::pair<std::string, opcodes>("S_SENDALLCLIENTSNAME", &Server::sendAllClientsName));
	list.insert(std::pair<std::string, opcodes>("S_SENDPROFILE", &Server::sendProfile));
	list.insert(std::pair<std::string, opcodes>("S_GETCURRENTPOSITION", &Server::getPosition));

	std::cout << "OPCodes initialized: " << list.size() << " OPCodes loaded" << std::endl;

	this->items = new itemsManager();
	this->npcSpawn = new npcSpawner();
	this->messages = new messageManager();
	this->skills = new skillManager();
}

bool checkPlugins(std::string token, std::map<std::string, std::vector<std::string>> modulesConfiguration)
{
	auto tokenSide = token.substr(0, token.find('_') + 1);
	token.erase(0, tokenSide.length());
	auto tokenPrefix = token.substr(0, token.find('_') + 1);

	if (modulesConfiguration.find(tokenPrefix) != modulesConfiguration.end())
	{
		return true;
	}
	return false;
}

void Server::processOpcodes(std::vector<std::string> opcodes, std::string ip)
{
	for (auto& cmd : opcodes)
	{
		std::string token = cmd.substr(0, cmd.find(':'));
		cmd.erase(0, token.length() + 1);
		std::vector<std::string> args = formatString(cmd);

		if (this->list.find(token) != this->list.end())
		{
			(this->*list[token])(args);
		}
		else if (checkPlugins(token, this->modulesConfiguration))
		{
			processPlugins(token, cmd, args);
		}
		else
		{
			std::cerr << "Invalid opcode sent from: " << ip << std::endl;
		}
		args.clear();
	}
}

void relay(SOCKET serverRCV, char* buffer, SOCKADDR_IN ipep)
{
	sendto(serverRCV, buffer, strlen(buffer), 0, (struct sockaddr*)&ipep, sizeof(ipep));
}

void Server::processPlugins(std::string token, std::string cmd, std::vector<std::string> args)
{
	auto tokenSide = token.substr(0, token.find('_') + 1);
	token.erase(0, tokenSide.length());
	auto tokenPrefix = token.substr(0, token.find('_') + 1);
	
	char buffer[4024] = "";
	std::string port;
	std::string ip;

	if (!this->modulesConfiguration.count(tokenPrefix))
		return;

	if (tokenSide[0] == 'S')
	{
		port = this->modulesConfiguration[tokenPrefix][1];
		ip = this->modulesConfiguration[tokenPrefix][0];
	}
	else if (tokenSide[0] == 'C')
	{
		port = args[args.size() - 1];
		ip = args[args.size() - 2];
	}

	this->ipep.sin_family = AF_INET;
	this->ipep.sin_addr.s_addr = inet_addr(ip.c_str());
	this->ipep.sin_port = htons(std::stoi(port));

	cmd = token + ':' + cmd;
	cmd += "0x12\n";

	strcpy_s(buffer, cmd.c_str());

	relay(this->serverRCV, buffer, this->ipep);
}