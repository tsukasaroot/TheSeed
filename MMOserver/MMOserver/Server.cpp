#include "Server.h"

std::map<std::string, std::vector<std::string>> Server::getServerConfig()
{
	auto reader = new xmlParser("config/serverConfig.xml");
	auto config = stockXML(reader);

	this->abnormalitiesTolerance = std::stoi(config["abnormalitiesTolerance"][0]);

	auto moduleList = new xmlParser("config/listingPlugins.xml");
	auto modules = stockXML(moduleList);

	for (auto it = modules["moduleList"].begin(); it != modules["moduleList"].end(); it++)
	{
		if (config.count(*it))
		{
			std::vector<std::string> values;
			std::string name = *it;

			values.push_back(config[name][1]);
			values.push_back(config[name][2]);

			this->modulesConfiguration.insert(std::pair<std::string, std::vector<std::string>>(config[name][0], values));
		}
	}

	this->salt = generateSalt(config["salt"][0]);

	/**
	** To add the debug option
	**/

	/*for (auto it = modulesConfiguration.begin(); it != modulesConfiguration.end(); it++)
	{
		std::cout << std::endl << "Key : " << it->first << " Values : ";

		for (auto sec = it->second.begin(); sec != it->second.end(); sec++)
		{
			std::cout << *sec << " - ";
		}
		std::cout << std::endl;
	}*/

	return config;
}

Server::Server()
{
	dataBase = new SQLManager();

	auto config = getServerConfig();

	this->serverPort = std::stoi(config["port"][0]);

	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 1;

	error = WSAStartup(MAKEWORD(2, 2), &initialisation_win32);
	if (error != 0)
		std::cout << "Can't initialize Winsock : " << error << " " << WSAGetLastError() << std::endl;
	else
		std::cout << std::endl << "WSAStartup : OK" << std::endl;

	this->serverRCV = socket(AF_INET, SOCK_DGRAM, 0);
	if (this->serverRCV == INVALID_SOCKET)
		std::cout << "Can't initialiaze socket : " << WSAGetLastError() << std::endl;
	else
		std::cout << "socket : OK" << std::endl;

	ipep.sin_family = AF_INET;
	ipep.sin_addr.s_addr = INADDR_ANY;
	ipep.sin_port = htons(this->serverPort);
	error = bind(this->serverRCV, (struct sockaddr*)&ipep, sizeof(ipep));
	if (error != 0)
		std::cout << "Can't listen to this port : " << error << " " << WSAGetLastError() << std::endl;
	else
		std::cout << "Bind : OK" << std::endl;
	std::cout << std::endl;
	
	// Will initialiaze the different XMLs needed for the game
	Opcodesinitialize();
	std::cout << std::endl << "$ Main server ready $" << std::endl << "Listening on: " << this->serverPort << std::endl << std::endl;
}

void Server::closeServer()
{
	error = closesocket(this->serverRCV);
	if (error != 0)
		std::cout << "Socket can't be freed : " << error << " " << WSAGetLastError() << std::endl;
	else
		std::cout << "close socket : OK" << std::endl;
	 
	error = WSACleanup();
	if (error != 0)
		std::cout << "Winsock can't be freed : " << error << " " << WSAGetLastError() << std::endl;
	else
		std::cout << "WSACleanup  : OK" << std::endl;
	exit(0);
}

void Server::saveWorld()
{
	for (auto it = this->_client.begin(); it != this->_client.end(); it++)
	{
		it->second->saveClientToDatabase();
	}
}

void Server::clientChecks()
{
	for (auto it = this->_client.begin(); it != this->_client.end(); it++)
	{
		it->second->queryClientPosition();
	}
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