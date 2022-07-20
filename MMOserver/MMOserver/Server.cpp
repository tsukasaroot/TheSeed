#include "Server.h"

void Server::getServerConfig()
{
	const char* path = "config/serverConfig.xml";
	xml_document<> doc;
	xml_node<>* root_node = NULL;

	if (!std::filesystem::exists(path))
	{
		std::cerr << "File " << path << " not found" << std::endl;
		exit(-1);
	}

	std::ifstream theFile(path);

	std::cout << "test";

	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);
	root_node = doc.first_node("Servers");

	if (xml_node<>* node = root_node->first_node("WorldServer"))
	{
		this->serverPort = std::stoi(node->first_attribute("port")->value());
		this->abnormalitiesTolerance = std::stoi(node->first_attribute("packetAbnormalsPerClient")->value());
		this->salt = node->first_attribute("opcodeSalt")->value();
	}
}

Server::Server()
{
	getServerConfig();

	dataBase = new SQLManager();

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