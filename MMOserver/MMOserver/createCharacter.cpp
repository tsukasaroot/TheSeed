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
	auto it = std::find(this->playerList.begin(), this->playerList.end(), currentClient);

	if (it == this->playerList.end())
	{
		return;
	}

	if (this->_client[cmd["id"]]->getState() != ISLOBBY) 
	{
		this->_client[cmd["id"]]->clientWrite("C_LOBBY:character doesn't exist");
	}

	this->_client[cmd["id"]]->setState(ISCREATECHAR);

	PlayerSlider temp_slider;

	if (!this->dataBase->is_name_valid(cmd["name"]))
	{
		this->_client[currentClient]->clientWrite("S_CREATE_CHAR:error{name is taken}:name{" + cmd["name"] + "}");
		return;
	}

	for (auto it = cmd.begin(); it != cmd.end(); it++)
	{
		if (it->first == "port" || it->first == "id" || it->first == "ip" || it->first == "name" || it->first == "class")
			continue;
		else
			temp_slider[it->first] = it->second;
	}

	this->_client[currentClient]->setSliders(temp_slider);

	this->dataBase->registerNewCharacter(currentClient, cmd["name"], cmd["class"], temp_slider);
	this->_client[cmd["id"]]->setState(ISLOBBY);
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