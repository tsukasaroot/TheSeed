/*** Make up the logic to create characters
* 
*/

#include "Server.h"

void Server::createCharacters(std::map<std::string, std::string> cmd)
{
	if (checkAll(10, cmd, &this->playerList))
	{
		return;
	}

	std::string currentClient = cmd["id"];

	if (!this->dataBase->is_name_valid(cmd["name"]))
	{
		this->_client[currentClient]->clientWrite("S_CREATE_CHAR:error{name is taken}");
		return;
	}

	for (auto it = cmd.begin(); it != cmd.end(); it++)
	{
		this->sliders[it->first][it->second]["activated"] = "true";
		std::cout << "customization " << it->first << " validated" << std::endl;
	}
}

void Server::checkNameValidity(std::map<std::string, std::string> cmd)
{
	if (!checkAll(4, cmd, &this->playerList))
	{
		return;
	}
	std::string currentClient = cmd["id"];
	std::string listName = "C_ISNAME_VALID";

	if (this->dataBase->is_name_valid(cmd["name"]))
	{
		this->_client[currentClient]->clientWrite(listName + ":true");
		return;
	}
	this->_client[currentClient]->clientWrite(listName + ":false");
}