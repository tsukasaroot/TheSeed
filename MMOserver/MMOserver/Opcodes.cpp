#include "Server.h"

void Server::Opcodesinitialize()
{
	list.insert(std::pair<std::string, opcodes>("login", &Server::login));
	list.insert(std::pair<std::string, opcodes>("logout", &Server::logout));
}