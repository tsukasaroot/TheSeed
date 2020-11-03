#ifndef SERVER_H_
#define SERVER_H_

#include "Client.h"
#include "SQLmanager.h"
#include "xmlParser.h"

class Server {
public:
	Server();
	void Start(std::vector<std::string> opcodes, std::string ip);

	SOCKET getSocket();
	struct timeval getTimeVal();
	SOCKADDR_IN getIpep();
private:
	int serverPort = 11102;
	struct timeval read_timeout;
	WSADATA initialisation_win32; // Variable permettant de r�cup�rer la structure d'information sur l'initialisation
	int error; // Variable permettant de r�cup�rer la valeur de retour des fonctions utilis�es
	SOCKET serverRCV;
	SOCKADDR_IN ipep; // D�claration de la structure des informations li� � l'�coute

	void login(std::vector<std::string> cmd);
	void logout(std::vector<std::string> cmd);
	void Opcodesinitialize();
	void closeServer();
	void savePlayerData(std::vector<std::string> cmd);

	typedef void(Server::*opcodes)(std::vector<std::string>);
	std::map<std::string, opcodes> list;

	xmlParser *reader;
	std::map<std::string, Client*> _client;
	std::map<std::string, std::string> Movements;
	std::vector<std::pair<std::string, Client>> client;
	SQLManager *dataBase;
};

#endif