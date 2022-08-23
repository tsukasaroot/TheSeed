/*** Make up the logic to create characters
* 
*/

#include "Server.h"

void Server::createCharacters(std::map<std::string, std::string> cmd)
{
	if (checkAll(10, cmd, &this->playerList))
	{
		if (!this->dataBase->is_name_valid(cmd["name"]))
		{

		}
		// probably all of the below should be loaded at start from datasheets tehe
		// retrieve all disponible class
		// retrieve all disponible races
		// retrieve all id's for every part of bodies possible
		// retrieve voices all this will be done for verifications, we don't need to send any of that to client 
	}
}

void Server::checkNameValidity(std::map<std::string, std::string> cmd)
{
	if (checkAll(4, cmd, &this->playerList))
	{
		std::string currentClient = cmd["id"];
		std::string listName = "C_ISNAME_VALID";

		if (this->dataBase->is_name_valid(cmd["name"]))
		{
			this->_client[currentClient]->clientWrite(listName + ":true");
			return;
		}
		this->_client[currentClient]->clientWrite(listName + ":false");
	}
}	