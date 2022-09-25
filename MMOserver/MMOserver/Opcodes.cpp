#include "Server.h"

void Server::Opcodesinitialize()
{
	list.insert(std::pair<std::string, opcodes>("S_LOGIN", &Server::login));
	list.insert(std::pair<std::string, opcodes>("S_EXIT", &Server::client_exit));
	list.insert(std::pair<std::string, opcodes>("S_LOGOUT", &Server::client_logout));
	list.insert(std::pair<std::string, opcodes>("S_SAVEDATA", &Server::savePlayerData));
	list.insert(std::pair<std::string, opcodes>("S_SENDCLIENTDATA", &Server::sendClientData));
	list.insert(std::pair<std::string, opcodes>("S_PRIVATEMESSAGE", &Server::privateMessage));
	list.insert(std::pair<std::string, opcodes>("S_GLOBALMESSAGE", &Server::globalMessage));
	list.insert(std::pair<std::string, opcodes>("S_SENDALLCLIENTSNAME", &Server::sendAllClientsName));
	list.insert(std::pair<std::string, opcodes>("S_SENDPROFILE", &Server::sendProfile));
	list.insert(std::pair<std::string, opcodes>("S_GETCURRENTPOSITION", &Server::getPosition));
	list.insert(std::pair<std::string, opcodes>("S_LOBBY", &Server::inLobby));
	list.insert(std::pair<std::string, opcodes>("S_CREATE_CHAR", &Server::createCharacters));
	list.insert(std::pair<std::string, opcodes>("S_CHECK_NAME_ISVALID", &Server::checkNameValidity));

	std::cout << "OPCodes initialized: " << list.size() << " OPCodes loaded" << std::endl;
	std::cout << std::endl;

	this->items = new itemsManager();
	std::cout << std::endl;
	this->npcSpawn = new npcSpawner();
	std::cout << std::endl;
	this->messages = new messageManager();
	std::cout << std::endl;
	this->skills = new skillManager();
	std::cout << std::endl;

	std::cout << std::endl;

	xml_document<> doc;
	std::vector<char> buffer;
	
	buffer = openXml("datasheets/class.xml");
	doc.parse<0>(&buffer[0]);

	auto class_xml = doc.first_node();

	for (xml_node<>* node = class_xml->first_node(); node; node = node->next_sibling())
	{
		this->classes.push_back(std::pair<std::string, int>("id", std::stoi(node->first_attribute("id")->value())));
	}

	doc.clear();
	
	buffer = openXml("datasheets/races.xml");
	doc.parse<0>(&buffer[0]);
	auto races_xml = doc.first_node();

	for (xml_node<>* node = races_xml->first_node(); node; node = node->next_sibling())
	{
		this->classes.push_back(std::pair<std::string, int>("id", std::stoi(node->first_attribute("id")->value())));
	}

	doc.clear();
	buffer = openXml("datasheets/customization.xml");
	doc.parse<0>(&buffer[0]);
	auto customization_xml = doc.first_node();

	for (xml_node<>* node = customization_xml->first_node(); node; node = node->next_sibling())
	{
		this->sliders[node->name()][node->first_attribute("id")->value()]["x"] = "125500.000";
		this->sliders[node->name()][node->first_attribute("id")->value()]["y"] = "125500.000";
		this->sliders[node->name()][node->first_attribute("id")->value()]["z"] = "125500.000";
	}
	
	if (this->sliders["head"].count("1"))
	{
		std::cout << "basic head_1 exists debug method to find customization values" << std::endl;
	}
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
		std::map<std::string, std::string> args = formatStringAssociative(cmd);

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

void Server::processPlugins(std::string token, std::string cmd, std::map<std::string, std::string> args)
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
		port = args["ip"];
		ip = args["port"];
	}

	this->ipep.sin_family = AF_INET;
	this->ipep.sin_addr.s_addr = inet_addr(ip.c_str());
	this->ipep.sin_port = htons(std::stoi(port));

	cmd = token + ':' + cmd;
	cmd += "0x12\n";

	strcpy_s(buffer, cmd.c_str());

	relay(this->serverRCV, buffer, this->ipep);
}