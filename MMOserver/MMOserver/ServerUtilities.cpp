#include "Server.h"

void Server::logout(std::vector<std::string> cmd)
{
	std::string nickName = cmd[0];
	this->_client[nickName]->closeClient();
	delete(this->_client[nickName]);
	this->_client.erase(nickName);
}

void Server::login(std::vector<std::string> cmd)
{
	std::cerr << "login here" << std::endl;
	std::string login = cmd[0];
	std::string password = cmd[1];
	auto result = this->dataBase->checkLogin(login);
	if (login == result["name"] && password == result["password"])
		this->_client.insert(std::pair<std::string, Client*>(login, new Client()));
	_client[login]->initClient(cmd[2], result["name"]);

	auto datas = this->dataBase->initPlayer(login);
	_client[login]->initClient(datas);
}

void Server::savePlayerData(std::vector<std::string> cmd)
{
	std::vector<std::pair<std::string, std::string>> values;

	values.push_back(std::make_pair("email", "jorouco@free.fr"));
	dataBase->update("test", "users", values);
}