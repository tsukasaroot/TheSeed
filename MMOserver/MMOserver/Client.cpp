#include "Client.h"

Client::Client(std::string client, std::string nickName)
{
	std::cout << "New client logging: " << nickName << std::endl;
	this->error = WSAStartup(MAKEWORD(2, 2), &initialisation_win32);
	if (this->error != 0)
		std::cout << "Can't initialize Winsock : " << this->error << " " << WSAGetLastError() << std::endl;

	this->_client = socket(AF_INET, SOCK_DGRAM, 0);
	if (this->_client == INVALID_SOCKET)
		std::cout << "Can't initialiaze socket : " << WSAGetLastError() << std::endl;

	this->clientAddress = client;
	this->nickName = nickName;

	clientWrite("Accepted");
}

void Client::initClient(std::string client, std::string nickName)
{
	std::cout << "New client logging: " << nickName << std::endl;
	this->error = WSAStartup(MAKEWORD(2, 2), &initialisation_win32);
	if (this->error != 0)
		std::cout << "Can't initialize Winsock : " << this->error << " " << WSAGetLastError() << std::endl;

	this->_client = socket(AF_INET, SOCK_DGRAM, 0);
	if (this->_client == INVALID_SOCKET)
		std::cout << "Can't initialiaze socket : " << WSAGetLastError() << std::endl;

	this->clientAddress = client;
	this->nickName = nickName;

	clientWrite("Accepted");
}

void Client::initClient(std::map<std::string, std::string> cmd)
{
	this->HP = std::stod(cmd["hp"]);
	this->x = std::stod(cmd["x"]);
	this->y = std::stod(cmd["y"]);
	this->z = std::stod(cmd["z"]);
	this->MP = std::stoi(cmd["mp"]);
	this->RE = std::stoi(cmd["re"]);
	this->clientClass = std::stoi(cmd["class"]);
}

void Client::closeClient()
{
	this->error = closesocket(this->_client);
	if (this->error != 0)
		std::cout << "Socket can't be freed : " << this->error << " " << WSAGetLastError() << std::endl;

	this->error = WSACleanup();
	if (this->error != 0)
		std::cout << "Winsock can't be freed : " << this->error << " " << WSAGetLastError() << std::endl;
	std::cout << "Client logged out: " << this->nickName << std::endl;
}

void Client::clientWrite(std::string msg)
{
	this->ipep.sin_family = AF_INET;
	this->ipep.sin_addr.s_addr = inet_addr(this->clientAddress.c_str()); // Indique l'adresse IP du client qui a �t� push
	this->ipep.sin_port = htons(11101);
	msg = msg + "0x12";
	strcpy_s(buffer, msg.c_str());
	this->bytes = sendto(this->_client, buffer, strlen(buffer), 0, (struct sockaddr*)&this->ipep, sizeof(this->ipep));
	if (this->bytes == SOCKET_ERROR)
		std::cout << "Can't send data: " << WSAGetLastError() << std::endl;
}

void Client::forward()
{
	this->x += 1;
	clientWrite("movement:forward:" + std::to_string(this->x) + ";" + std::to_string(this->y) + ";" + std::to_string(this->z));
}

void Client::back()
{
	this->x -= 1;
	clientWrite("movement:backward:" + std::to_string(this->x) + ";" + std::to_string(this->y) + ";" + std::to_string(this->z));
}

void Client::right()
{
	this->y += 1;
	clientWrite("movement:right:" + std::to_string(this->x) + ";" + std::to_string(this->y) + ";" + std::to_string(this->z));
}

void Client::left()
{
	this->y -= 1;
	clientWrite("movement:left:" + std::to_string(this->x) + ";" + std::to_string(this->y) + ";" + std::to_string(this->z));
}

/*
** Getters method
*/

SOCKET Client::getClientSocket()
{
	return this->_client;
}

std::string Client::getClientAddress()
{
	return this->clientAddress;
}

std::string Client::getNickName()
{
	return this->nickName;
}

double Client::getX()
{
	return this->x;
}

double Client::getY()
{
	return this->y;
}

double Client::getZ()
{
	return this->z;
}

std::string Client::getAll()
{
	std::string toSend;

	std::cout << this->MP << ':' << this->clientClass << ':' << this->RE << ':' + this->HP << ':' + this->currency << std::endl;
	toSend = this->MP + ':' + this->clientClass + ':' + this->RE + ':' + this->HP + ':' + this->currency;
	return toSend;
}