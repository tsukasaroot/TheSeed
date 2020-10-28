#ifndef CLIENT_H_
#define CLIENT_H_

#include <windows.h>

#include <iostream>
#include <string>
#include <thread>

class Client {
public:
	Client(std::string client, std::string nickName);
	void closeClient();

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
	double x, y, z;

	SOCKET _client;

	WSADATA initialisation_win32; // Variable permettant de r�cup�rer la structure d'information sur l'initialisation
	int error, tempo, bytes;
	char buffer[4024]; // Tampon contennant les donn�es re�ues ou envoy�es
	SOCKADDR_IN ipep; // D�claration de la structure des informations li� au serveur
};

#endif