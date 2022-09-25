#ifndef SERVER_H_
#define SERVER_H_

#include "Client.h"
#include "SQLmanager.h"
#include "npcSpawner.h"
#include "itemManager.h"
#include "messageManager.h"
#include "skillManager.h"

/*#ifdef __linux__ 
//linux code goes here
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#elif _WIN32*/
// windows code goes here
#include <winsock.h>
//#endif

/*
* Check if opcode contain the right amount of args and if a valid player exist
* 
* @param size of the command, the command itself and the player list contained in class Server
* @return a boolean depending if all test succeed
*/

bool checkAll(int size, std::map<std::string, std::string> cmd, std::vector<std::string>* playerList);
std::string decipherPacket(std::string toDecipher, std::string salt);
std::string generateSalt(std::string salt);
std::map<std::string, std::string> formatStringAssociative(std::string line);
std::vector<char> openXml(const char* path);

class Server {
public:
	Server();
	void processOpcodes(std::vector<std::string> opcodes, std::string ip);
	void clientChecks();
	void saveWorld();
	void createAI();

	SOCKET getSocket();
	struct timeval getTimeVal();
	SOCKADDR_IN getIpep();

	std::string salt;
private:
	void getServerConfig();

	int serverPort = 16384;
	struct timeval read_timeout;
	WSADATA initialisation_win32;
	int error;
	SOCKET serverRCV;
	SOCKADDR_IN ipep;

	/**
	* Log the client server-side.
	*
	* @param Take a cmd string that contain IP, nickname and password.
	* @return no return values.
	*/
	void login(std::map<std::string, std::string> cmd);
	/**
	* Logout the client server-side.
	*
	* @param Take a cmd string that contain IP, nickname.
	* @return no return values.
	*/
	void client_exit(std::map<std::string, std::string> cmd);
	/**
	* Save players data server-side.
	*
	* @param Take a cmd string that contain all clients informations.
	* @return no return values.
	*/
	void savePlayerData(std::map<std::string, std::string> cmd);
	/**
	* Get Client Data server-side.
	*
	* @param Take a cmd string that contain clients info we want and send info back.
	* @return no return values.
	*/
	void sendClientData(std::map<std::string, std::string> cmd);
	/**
	* Send a private message to a connected client
	* 
	* @param Take the current client that wants to send the message, the second client that will receive it, and finally the string
	* @return no return values
	*/
	void privateMessage(std::map<std::string, std::string> cmd);
	/**
	* Send a global message to all connected client
	*
	* @param Take the current client that wants to send the message and finally the string
	* @return no return values
	*/
	void globalMessage(std::map<std::string, std::string> cmd);
	/**
	* Send to the asking client all the connected clients name
	*
	* @param Take the current client that wants the list
	* @return no return values
	*/
	void sendAllClientsName(std::map<std::string, std::string> cmd);
	/**
	* Send a profile data to the client asking for it
	*/
	void sendProfile(std::map<std::string, std::string> cmd);
	/**
	* Check a player's position to update it on the server + checking for speedhacks / tps
	*/
	void getPosition(std::map<std::string, std::string> cmd);
	/**
	* Initialize the opcodes
	*/
	void Opcodesinitialize();
	/**
	* Process sending packets to related servers
	*/
	void processPlugins(std::string token, std::string cmd, std::map<std::string, std::string> args);
	/**
	* Shutdown server properly by saving all data
	*/
	void closeServer();

	/** 
	* switch player status to lobby and send all existing characters linked to his account
	*/
	void inLobby(std::map<std::string, std::string> cmd);

	/**
	* When player send validation to create character, server checked all data then insert in database
	*/
	void createCharacters(std::map<std::string, std::string> cmd);

	/**
	* Simply check if a name already exist amongst players and return a boolean in the packet
	*/
	void checkNameValidity(std::map<std::string, std::string> cmd);

	void client_logout(std::map<std::string, std::string> cmd);

	typedef void(Server::* opcodes)(std::map<std::string, std::string>);

	std::map<std::string, opcodes> list;
	std::vector<std::string> playerList;

	skillManager* skills;
	messageManager* messages;
	itemsManager* items;
	npcSpawner* npcSpawn;
	std::map<std::string, Client*> _client;
	std::map<std::string, std::string> Movements;
	SQLManager *dataBase;
	std::vector<std::pair<std::string, int>> classes;
	std::vector<std::pair<std::string, int>> races;
	std::unordered_map<std::string, std::map<std::string, std::unordered_map<std::string, std::string>>> sliders;

	int abnormalitiesTolerance = 10;
	std::map<std::string, std::vector<std::string>> modulesConfiguration;
};

#endif