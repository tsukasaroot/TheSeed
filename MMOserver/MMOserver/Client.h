#ifndef CLIENT_H_
#define CLIENT_H_

#include <windows.h>

#include <iostream>
#include <string>
#include <thread>
#include <map>

class Client {
public:
	Client(std::string client, std::string nickName);
	Client() {};
	void closeClient();
	void initClient(std::string client, std::string nickName);
	void initClient(std::map<std::string, std::string> cmd);

	void clientWrite(std::string msg);
	void forward();
	void back();
	void left();
	void right();

	double getX();
	double getY();
	double getZ();

	SOCKET getClientSocket();
	std::string getClientAddress();
	std::string getNickName();
private:
	std::string clientAddress;
	std::string nickName;
	double x = 0, y = 0, z = 0, HP = 0;
	int MP = 0, RE = 0;

	SOCKET _client = 0;

	WSADATA initialisation_win32; // Variable permettant de récupérer la structure d'information sur l'initialisation
	int error = 0, tempo = 0, bytes = 0;
	char buffer[4024] = ""; // Tampon contenant les données reçues ou envoyées
	SOCKADDR_IN ipep; // Déclaration de la structure des informations lié au serveur
};

#endif