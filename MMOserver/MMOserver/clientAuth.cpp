#include "Server.h"

// Gotta need to send and check token validity, else anyone can disconnect another user if IP check if disabled lmao
void Server::client_exit(std::map<std::string, std::string> cmd)
{
	if (cmd.size() == 3)
	{
		std::string token = cmd["id"];
		auto it = std::find(this->playerList.begin(), this->playerList.end(), token);

		if (it != this->playerList.end())
		{
			this->playerList.erase(it);
			this->_client[token]->closeClient();
			delete(this->_client[token]);
			this->_client.erase(token);
			return;
		}
		std::cerr << "Error: " << token << " player already disconnected, logout command denied." << std::endl;
		return;
	}
	else
	{
		std::cerr << "Not enough argument for this function" << std::endl;
	}
}

void Server::client_logout(std::map<std::string, std::string> cmd)
{
	if (cmd.size() == 3)
	{
		std::string token = cmd["id"];
		auto it = std::find(this->playerList.begin(), this->playerList.end(), token);

		if (it != this->playerList.end())
		{
			if (this->_client[token]->getState() == ISLOBBY)
			{
				return;
			}
			this->_client[token]->logout();
			auto characters = this->dataBase->retrieve_all_chars(std::stoi(token));
			std::vector<std::string> packets_array = {
				"C_LOBBY", "c_number{" + std::to_string(characters.size()) + "}"
			};

			for (int i = 0; i < characters.size(); i++)
			{
				packets_array.push_back("[");

				packets_array.push_back("id{" + characters[i]["player_id"] + "}");
				packets_array.push_back("name{" + characters[i]["name"] + "}");
				packets_array.push_back("class{" + characters[i]["class"] + "}");
				packets_array.push_back("lastEnterWorld{" + characters[i]["lastEnterWorld"] + "}");
				packets_array.push_back("level{" + characters[i]["level"] + "}");

				packets_array.push_back("]");
			}
			auto packet = packetBuilder(packets_array);

			_client[token]->clientWrite(packet);
		}
	}
}

void Server::login(std::map<std::string, std::string> cmd)
{
	if (cmd.size() == 3)
	{
		std::string token = cmd["token"];
		auto result = this->dataBase->checkLogin(token);
		
		if (result.size() > 0)
		{
			auto it = std::find(this->playerList.begin(), this->playerList.end(), result["account_id"]);
			if (it != this->playerList.end())
			{
				std::cerr << "player is already connected " << result["account_id"] << std::endl;
				this->_client[result["account_id"]]->clientWrite("S_LOGIN:error{-1}");
				return;
			}
			this->playerList.push_back(result["account_id"]);
			this->_client.insert(std::pair<std::string, Client*>(result["account_id"], new Client()));
			_client[result["account_id"]]->initClient(cmd["ip"], result, this->serverRCV, this->dataBase, cmd["port"]);

			auto characters = this->dataBase->retrieve_all_chars(std::stoi(result["account_id"]));
			std::vector<std::string> packets_array = {
				"C_LOBBY", "c_number{" + std::to_string(characters.size()) + "}"
			};

			for (int i = 0; i < characters.size(); i++)
			{
				packets_array.push_back("[");

				packets_array.push_back("id{" + characters[i]["player_id"] + "}");
				packets_array.push_back("name{" + characters[i]["name"] + "}");
				packets_array.push_back("class{" + characters[i]["class"] + "}");
				packets_array.push_back("lastEnterWorld{" + characters[i]["lastEnterWorld"] + "}");
				packets_array.push_back("level{" + characters[i]["level"] + "}");

				packets_array.push_back("]");
			}
			auto packet = packetBuilder(packets_array);

			_client[result["account_id"]]->clientWrite(packet);
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

void Server::savePlayerData(std::map<std::string, std::string> cmd)
{
	std::vector<std::pair<std::string, std::string>> values;
	std::string player_id = cmd["id"];

	values.push_back(std::make_pair("x", "8000"));
	values.push_back(std::make_pair("y", "8000"));
	values.push_back(std::make_pair("z", "8000"));
	values.push_back(std::make_pair("region", "0"));
	values.push_back(std::make_pair("isAlive", "1"));
	values.push_back(std::make_pair("currency", "1000"));

	this->dataBase->update(cmd["id"], "player_id", "users", values);
}