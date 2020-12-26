#ifndef SERVER_H_
#define SERVER_H_

#include "Client.h"
#include "SQLmanager.h"
#include "xmlParser.h"
#include "inventoryManager.h"
#include "npcSpawner.h"
#include "itemManager.h"

class Server {
public:
	Server();
	void processOpcodes(std::vector<std::string> opcodes, std::string ip);

	SOCKET getSocket();
	struct timeval getTimeVal();
	SOCKADDR_IN getIpep();
private:
	int serverPort = 11102;
	struct timeval read_timeout;
	WSADATA initialisation_win32; // Variable permettant de récupérer la structure d'information sur l'initialisation
	int error; // Variable permettant de récupérer la valeur de retour des fonctions utilisées
	SOCKET serverRCV;
	SOCKADDR_IN ipep; // Déclaration de la structure des informations lié à l'écoute

	/**
	* Log the client server-side.
	*
	* @param Take a cmd string that contain IP, nickname and password.
	* @return no return values.
	*/
	void login(std::vector<std::string> cmd);
	/**
	* Logout the client server-side.
	*
	* @param Take a cmd string that contain IP, nickname.
	* @return no return values.
	*/
	void logout(std::vector<std::string> cmd);
	/**
	* Save players data server-side.
	*
	* @param Take a cmd string that contain all clients informations.
	* @return no return values.
	*/
	void savePlayerData(std::vector<std::string> cmd);
	/**
	* get Client Data server-side.
	*
	* @param Take a cmd string that contain clients info we want and send info back.
	* @return no return values.
	*/
	void getClientData(std::vector<std::string> cmd);

	void Opcodesinitialize();
	void closeServer();

	typedef void(Server::*opcodes)(std::vector<std::string>);
	std::map<std::string, opcodes> list;
	std::vector<std::string> playerList;

	itemsManager* items;
	inventoryManager* inventory;
	npcSpawner* npcSpawn;
	xmlParser* reader;
	std::map<std::string, Client*> _client;
	std::map<std::string, std::string> Movements;
	std::vector<std::pair<std::string, Client>> client;
	SQLManager *dataBase;
};

#endif