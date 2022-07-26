#ifndef CLIENT_H_
#define CLIENT_H_

#include "inventoryManager.h"
#include "SQLmanager.h"
#include "enum.h"

#include <rapidxml/rapidxml.hpp>
using namespace rapidxml;

#include <windows.h>
#include <bitset>
#include <iostream>
#include <string>
#include <thread>
#include <map>
#include <wincrypt.h>
#include <stdlib.h>

std::string packetBuilder(std::vector<std::string> requests);
std::string cipherPacket(std::string toBinaries, std::string salt);
std::string generateSalt(std::string salt);

class Client {
public:
	Client() {};
	~Client() {};
	void closeClient();
	void initClient(std::string ip, std::map<std::string, std::string> result, SOCKET client, SQLManager* db, std::string port);
	void initClient(std::map<std::string, std::string> player_data);

	void clientWrite(std::string msg);

	void queryClientPosition();

	double getX();
	double getY();
	double getZ();

	void setPositionQuery(std::vector<std::string> cmd);
	void saveClientToDatabase();
	void setState(WherePlayer state);

	std::string getAll();
	std::string getProfile();

	int getAbnormalities();
	SOCKET getClientSocket();
	std::string getClientAddress();
	std::string getNickName();
	int getState();
private:
	int port = 16384;
	std::string clientAddress;
	std::string nickName;
	std::string ip;
	double x = 0, y = 0, z = 0, HP = 0, currency = 0, exp = 0, attack = 0, critRate = 0,
		critP = 0, defense = 0;
	int MP = 0, RE = 0, clientClass = 0, positionQuery = 0, region = 0, level = 0;
	bool isAlive = true;
	int state = 0;
	unsigned int account_id, player_id;

	int abnormal = 0, modifier = 1;
	double movementTolerance = 6.1;

	inventoryManager* inventory;
	SQLManager* dataBase;

	SOCKET _client = 0;

	WSADATA initialisation_win32; // Variable permettant de récupérer la structure d'information sur l'initialisation
	int error = 0, tempo = 0, bytes = 0;
	 // Tampon contenant les données reçues ou envoyées
	SOCKADDR_IN ipep; // Déclaration de la structure des informations lié au serveur
	std::string salt;
};

#endif /* CLIENT_H_ */