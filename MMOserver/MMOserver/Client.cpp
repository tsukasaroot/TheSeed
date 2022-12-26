#include "Client.h"

void Client::initClient(std::string ip, std::map<std::string, std::string> result, SOCKET serverRCV, SQLManager *db, std::string port)
{
	std::cout << "New client logging: " << result["account_id"] << std::endl;

	this->dataBase = db;
	this->inventory = new inventoryManager(db);
	this->_client = serverRCV;
	this->clientAddress = ip;
	this->account_id = std::stoi(result["account_id"]);
	this->port = std::stoi(port);

	const char* path = "config/clientConfig.xml";

	xml_document<> doc;
	xml_node<>* root_node = NULL;

	if (!std::filesystem::exists(path))
	{
		std::cerr << "File " << path << " not found" << std::endl;
		exit(-1);
	}

	std::ifstream theFile(path);
	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);
	root_node = doc.first_node("Client");

	if (xml_node<>* student_node = root_node->first_node("movementTolerance"))
	{
		this->movementTolerance = std::stoi(student_node->first_attribute("value")->value());
	}

	if (xml_node<>* student_node = root_node->first_node("salt"))
	{
		this->salt = generateSalt(student_node->first_attribute("text")->value());
	}

	if (xml_node<>* student_node = root_node->first_node("debug"))
	{
		this->debug = to_bool(student_node->first_attribute("status")->value());

		if (this->debug) {
			std::cout << "We hardcode port to send to client" << std::endl;
			this->port = std::stoi(student_node->first_attribute("port")->value());
		}
	}

	this->state = ISLOBBY;
	clientWrite("C_LOGIN:id{" + std::to_string(this->account_id) + "}");
}

void Client::initClient(std::map<std::string, std::string> player_data)
{
	std::string packet_data;

	this->x = std::stod(player_data["x"]);
	this->y = std::stod(player_data["y"]);
	this->z = std::stod(player_data["z"]);
	this->currency = std::stod(player_data["currency"]);
	this->exp = std::stod(player_data["exp"]);

	this->HP = std::stod(player_data["hp"]);
	this->MP = std::stoi(player_data["mp"]);
	this->attack = std::stod(player_data["attack"]);
	this->critRate = std::stod(player_data["critRate"]);
	this->critP = std::stod(player_data["critP"]);
	this->defense = std::stod(player_data["defense"]);
	this->RE = std::stoi(player_data["re"]);

	this->clientClass = std::stoi(player_data["class"]);
	this->level = std::stoi(player_data["level"]);
	this->region = std::stoi(player_data["region"]);
	this->isAlive = std::stoi(player_data["isAlive"]);
	this->player_id = (unsigned)std::stoi(player_data["player_id"]);
	this->nickName = player_data["name"];

	std::vector<std::string> array_data = {
		"C_LOGIN_DATA", "name{" + this->nickName + "}", "id{" + std::to_string(this->player_id) + "}",
		"x{" + player_data["x"] + "}", "y{" + player_data["y"] + "}", "z{" + player_data["z"] + "}", "currency{" + player_data["currency"] + "}", "exp{" + player_data["exp"] + "}",
		"hp{" + player_data["hp"] + "}", "mp{" + player_data["mp"] + "}", "attack{" + player_data["attack"] + "}", "critRate{" + player_data["critRate"] + "}", "critP{" + player_data["critP"] + "}", "defense{" + player_data["defense"] + "}",
		"class{" + player_data["class"] + "}", "level{" + player_data["level"] + "}", "region{" + player_data["region"] + "}", "re{" + player_data["re"] + "}",
		"isAlive{" + player_data["isAlive"] + "}"
	};

	this->inventory->getClientInventory(this->player_id);

	packet_data = packetBuilder(array_data);
	this->clientWrite(packet_data);
}

void Client::closeClient()
{
	if (this->state == ISWORLDSERVER)
		saveClientToDatabase();
	std::cout << "Client disconnect: " << this->account_id << std::endl;
}


void Client::logout()
{
	if (this->state == ISWORLDSERVER)
		saveClientToDatabase();
	std::cout << "Client logout: " << this->account_id << std::endl;
	this->state = ISLOBBY;
	this->clientWrite("C_LOGOUT");
}

void Client::clientWrite(std::string msg)
{
	this->ipep.sin_family = AF_INET;
	this->ipep.sin_addr.s_addr = inet_addr(this->clientAddress.c_str()); // Indique l'adresse IP du client qui a été push
	this->ipep.sin_port = htons(this->port);
	msg = msg + "0x12";
	char buffer[4024] = "";

	std::string hashedPacket = cipherPacket(msg, this->salt);

	strcpy_s(buffer, hashedPacket.c_str());

	this->bytes = sendto(this->_client, buffer, strlen(buffer), 0, (struct sockaddr*)&this->ipep, sizeof(this->ipep));
	if (this->bytes == SOCKET_ERROR)
		std::cout << "Can't send data: " << WSAGetLastError() << std::endl;
	memset(buffer, 0, sizeof(buffer));
}

void Client::saveClientToDatabase()
{
	std::vector<std::pair<std::string, std::string>> values;

	values.push_back(std::make_pair((std::string)"x", std::to_string(this->x)));
	values.push_back(std::make_pair((std::string)"y", std::to_string(this->y)));
	values.push_back(std::make_pair((std::string)"z", std::to_string(this->z)));
	values.push_back(std::make_pair((std::string)"region", std::to_string(this->region)));
	values.push_back(std::make_pair((std::string)"currency", std::to_string(this->currency)));
	values.push_back(std::make_pair((std::string)"class", std::to_string(this->clientClass)));
	values.push_back(std::make_pair((std::string)"exp", std::to_string(this->exp)));
	values.push_back(std::make_pair((std::string)"isAlive", std::to_string(this->isAlive)));

	this->dataBase->update(std::to_string(this->player_id), "name", "users", values);
	values.clear();

	values.push_back(std::make_pair((std::string)"hp", std::to_string(this->HP)));
	values.push_back(std::make_pair((std::string)"mp", std::to_string(this->MP)));
	values.push_back(std::make_pair((std::string)"attack", std::to_string(this->attack)));
	values.push_back(std::make_pair((std::string)"critRate", std::to_string(this->critRate)));
	values.push_back(std::make_pair((std::string)"critP", std::to_string(this->critP)));
	values.push_back(std::make_pair((std::string)"defense", std::to_string(this->defense)));
	values.push_back(std::make_pair((std::string)"re", std::to_string(this->RE)));
	this->dataBase->update(std::to_string(this->player_id), "player_id", "currentplayerstats", values);
}

/*
** Setters method
*/

void Client::setSliders(PlayerSlider sliders)
{
	this->sliders = sliders;
}

void Client::setState(WherePlayer state)
{
	this->state = state;
}

void Client::setPositionQuery(std::map<std::string, std::string> cmd)
{
	this->positionQuery--;

	/*auto x = std::stod(cmd[1]);
	auto y = std::stod(cmd[2]);
	auto z = std::stod(cmd[3]);*/

	double xChecker = abs(x - this->x);
	double yChecker = abs(z - this->z);


	if (xChecker > this->movementTolerance)
	{
		this->abnormal++;
		clientWrite("C_CORRECTPOSITION:x:" + std::to_string(this->x));
	}
	else
	{
		this->x = x;
	}

	if (yChecker > this->movementTolerance)
	{
		this->abnormal++;
		clientWrite("C_CORRECTPOSITION:z:" + std::to_string(this->z));
	}
	else
	{
		this->z = z;
	}
}

/*
** Getters method
*/

int Client::getAbnormalities()
{
	return this->abnormal;
}

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

int Client::getState()
{
	return this->state;
}

std::string Client::getProfile()
{
	std::string toSend = this->nickName + ':' + getAll();

	return toSend;
}

std::string Client::getAll()
{
	std::string toSend;

	toSend = std::to_string(this->MP) + ':' + std::to_string(this->HP) + ':' + std::to_string(this->RE) + ':' + std::to_string(this->currency) + ':' + std::to_string(this->clientClass) + ':' +
		std::to_string(this->x) + ':' + std::to_string(this->y) + ':' + std::to_string(this->z);
	return toSend;
}