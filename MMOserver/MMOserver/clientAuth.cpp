#include "Server.h"

// Gotta need to send and check token validity, else anyone can disconnect another user lmao
void Server::logout(std::vector<std::string> cmd)
{
	if (cmd.size() == 3)
	{
		std::string token = cmd[0];
		auto it = std::find(this->playerList.begin(), this->playerList.end(), token);

		if (it != this->playerList.end())
		{
			this->playerList.erase(it);
			this->_client[token]->closeClient();
			delete(this->_client[token]);
			this->_client.erase(token);
			return;
		}
		std::cerr << "Error: " << token << " player already logged out, logout command denied." << std::endl;
		return;
	}
	else
	{
		std::cerr << "Not enough argument for this function" << std::endl;
	}
}

void Server::login(std::vector<std::string> cmd)
{
	if (cmd.size() == 3)
	{
		std::string token = cmd[0];
		auto result = this->dataBase->checkLogin(token);
		
		if (result.size() > 0)
		{
			auto it = std::find(this->playerList.begin(), this->playerList.end(), result["account_id"]);
			if (it != this->playerList.end())
			{
				std::cerr << "player is already connected " << result["account_id"] << std::endl;
				this->_client[result["account_id"]]->clientWrite("S_LOGIN:alreadylogged");
				return;
			}
			this->playerList.push_back(result["account_id"]);
			this->_client.insert(std::pair<std::string, Client*>(result["account_id"], new Client()));
			_client[result["account_id"]]->initClient(cmd[1], result, this->serverRCV, this->dataBase, cmd[2]);
			return;
		}
		else
		{
			std::cerr << "this account id " << token << " is unknown or already connected" << std::endl;
		}
	}
	else
	{
		std::cout << "Not enough argument for this function" << std::endl;
	}
}

void Server::inLobby(std::vector<std::string> cmd)
{
	if (this->_client[cmd[0]]->getState() == ISLOBBY)
	{
		std::string account_id = cmd[0];
		std::string player_id = cmd[1];
		auto datas = this->dataBase->initPlayer(account_id, player_id);

		std::cout << datas.size() << std::endl;

		if (datas.size() <= 1) {
			std::cerr << "abort" << std::endl;
			this->_client[account_id]->clientWrite("C_LOBBY:character doesn't exist");
			return;
		}

		this->_client[account_id]->initClient(datas);
		this->_client[account_id]->setState(ISWORLDSERVER);
	}
}

void Server::savePlayerData(std::vector<std::string> cmd)
{
	std::vector<std::pair<std::string, std::string>> values;
	std::string player_id = cmd[0];

	values.push_back(std::make_pair("x", "8000"));
	values.push_back(std::make_pair("y", "8000"));
	values.push_back(std::make_pair("z", "8000"));
	values.push_back(std::make_pair("region", "0"));
	values.push_back(std::make_pair("isAlive", "1"));
	values.push_back(std::make_pair("currency", "1000"));

	dataBase->update(cmd[0], "player_id", "users", values);
}