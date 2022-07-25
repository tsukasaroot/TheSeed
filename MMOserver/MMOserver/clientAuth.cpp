#include "Server.h"

// Gotta need to send and check token validity, else anyone can disconnect another user lmao
void Server::logout(std::vector<std::string> cmd)
{
	if (cmd.size() == 3)
	{
		std::string nickName = cmd[0];
		auto it = std::find(this->playerList.begin(), this->playerList.end(), nickName);

		if (it != this->playerList.end())
		{
			this->playerList.erase(it);
			this->_client[nickName]->closeClient();
			delete(this->_client[nickName]);
			this->_client.erase(nickName);
			return;
		}
		std::cerr << "Error: " << nickName << " player already logged out, logout command denied." << std::endl;
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
			this->playerList.push_back(result["player_id"]);
			this->_client.insert(std::pair<std::string, Client*>(result["player_id"], new Client()));
			_client[result["player_id"]]->initClient(cmd[1], result["name"], this->serverRCV, this->dataBase, cmd[2]);

			auto datas = this->dataBase->initPlayer(result["player_id"]);
			_client[result["player_id"]]->initClient(datas, result["player_id"]);
			return;
		}
		else
		{
			std::cerr << "this player id " << token << " is unknown" << std::endl;
		}
	}
	else
	{
		std::cout << "Not enough argument for this function" << std::endl;
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