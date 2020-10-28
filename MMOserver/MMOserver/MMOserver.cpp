#include "SQLmanager.h"
#include "Client.h"
#include "Server.h"

auto const tableName = "users";

int main(int argc, char* argv[])
{
	std::string delimiter = "0x12";
	std::vector<std::string> opcodes;

	std::vector<Client> client;

	char buffer[4024];
	int bytes;
	int buffLength = 4024;
	int tempo;

	Server *server = new Server();

	SOCKET serverRCV = server->getSocket();
	struct timeval read_timeout = server->getTimeVal();
	SOCKADDR_IN ipep = server->getIpep();

	while (1)
	{
		fd_set rfds;

		FD_ZERO(&rfds);
		FD_SET(serverRCV, &rfds);
		int recVal = select(serverRCV + 1, &rfds, NULL, NULL, &read_timeout);

		if (recVal != 0 && recVal != -1) {
			size_t pos = 0;

			tempo = sizeof(ipep);
			bytes = recvfrom(serverRCV, buffer, buffLength, 0, (struct sockaddr*)&ipep, &tempo);
			std::string ip = inet_ntoa(ipep.sin_addr);
			buffer[bytes] = 0;

			std::string line = buffer;
			while ((pos = line.find(delimiter)) != std::string::npos)
			{
				std::string token = line.substr(0, line.find(delimiter));
				token += ':' + ip;
				if (!token.empty())
					opcodes.push_back(token);
				line.erase(0, pos + delimiter.length());
				token.clear();
			}

			server->Start(opcodes, ip);

			memset(buffer, 0, buffLength);
			line.clear();
			opcodes.clear();
		}
	}
	return 0;
}