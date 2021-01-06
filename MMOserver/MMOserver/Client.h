#ifndef CLIENT_H_
#define CLIENT_H_

#include "inventoryManager.h"
#include "SQLmanager.h"

#include <windows.h>

#include <iostream>
#include <string>
#include <thread>
#include <map>

class Client {
public:
	Client() {};
	~Client() {};
	void closeClient();
	void initClient(std::string client, std::string nickName, SOCKET serverRCV, SQLManager* db);
	void initClient(std::map<std::string, std::string> cmd);

	void clientWrite(std::string msg);

	void queryClientPosition();

	void forward();
	void back();
	void left();
	void right();

	double getX();
	double getY();
	double getZ();

	std::string getAll();

	SOCKET getClientSocket();
	std::string getClientAddress();
	std::string getNickName();
private:
	std::string clientAddress;
	std::string nickName;
	std::string ip;
	double x = 0, y = 0, z = 0, HP = 0;
	int MP = 0, RE = 0, clientClass = 0;
	double currency = 0;

	inventoryManager* inventory;

	SOCKET _client = 0;

	WSADATA initialisation_win32; // Variable permettant de récupérer la structure d'information sur l'initialisation
	int error = 0, tempo = 0, bytes = 0;
	char buffer[4024] = ""; // Tampon contenant les données reçues ou envoyées
	SOCKADDR_IN ipep; // Déclaration de la structure des informations lié au serveur
};

#endif