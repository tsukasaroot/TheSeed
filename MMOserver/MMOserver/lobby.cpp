#include "Server.h"

void Server::inLobby(std::map<std::string, std::string> cmd)
{
	std::string token = cmd["id"];
	auto it = std::find(this->playerList.begin(), this->playerList.end(), token);

	if (it != this->playerList.end())
	{
		if (this->_client[cmd["id"]]->getState() == ISLOBBY)
		{
			std::string account_id = cmd["id"];
			std::string player_id = cmd["character_id"];
			auto datas = this->dataBase->initPlayer(account_id, player_id);

			if (datas.size() <= 1) {
				std::cerr << "abort" << std::endl;
				this->_client[account_id]->clientWrite("C_LOBBY:character doesn't exist");
				return;
			}

			this->_client[account_id]->initClient(datas);
			this->_client[account_id]->setState(ISWORLDSERVER);
		}
	}
}