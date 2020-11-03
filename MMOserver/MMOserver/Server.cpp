#include "Server.h"

Server::Server()
{
	dataBase = new SQLManager();

	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 10;

	error = WSAStartup(MAKEWORD(2, 2), &initialisation_win32);
	if (error != 0)
		std::cout << "Can't initialize Winsock : " << error << " " << WSAGetLastError() << std::endl;
	else
		std::cout << "WSAStartup : OK" << std::endl;

	this->serverRCV = socket(AF_INET, SOCK_DGRAM, 0);
	if (serverRCV == INVALID_SOCKET)
		std::cout << "Can't initialiaze socket : " << WSAGetLastError() << std::endl;
	else
		std::cout << "socket : OK" << std::endl;

	ipep.sin_family = AF_INET;
	ipep.sin_addr.s_addr = INADDR_ANY;
	ipep.sin_port = htons(serverPort);
	error = bind(serverRCV, (struct sockaddr*)&ipep, sizeof(ipep));
	if (error != 0)
		std::cout << "Can't listen to this port : " << error << " " << WSAGetLastError() << std::endl;
	else
		std::cout << "Bind : OK" << std::endl;
	std::cout << std::endl;
	
	// Will initialiaze the different XMLs needed for the game
	Opcodesinitialize();
}

void Server::Start(std::vector<std::string> opcodes, std::string ip)
{
	for (auto & cmd : opcodes)
	{
		std::string token = cmd.substr(0, cmd.find(':'));
		cmd.erase(0, token.length() + 1);
		std::vector<std::string> args = formatString(cmd);

		(this->*list[token])(args);

		args.clear();
	}
}

void Server::closeServer()
{
	error = closesocket(serverRCV);
	if (error != 0)
		std::cout << "Socket can't be freed : " << error << " " << WSAGetLastError() << std::endl;
	else
		std::cout << "close socket : OK" << std::endl;
	 
	error = WSACleanup();
	if (error != 0)
		std::cout << "Winsock can't be freed : " << error << " " << WSAGetLastError() << std::endl;
	else
		std::cout << "WSACleanup  : OK" << std::endl;
}

SOCKET Server::getSocket()
{
	return this->serverRCV;
}

struct timeval Server::getTimeVal()
{
	return this->read_timeout;
}

SOCKADDR_IN Server::getIpep()
{
	return this->ipep;
}