#include "SQLmanager.h"
#include "Client.h"
#include "Server.h"

auto const tableName = "users";
const int BUFFER_SIZE = 4024;
const std::string delimiter = "0x12";

void runOpcodes(Server* server, std::vector<std::string> opcodes, std::string ip)
{
	server->processOpcodes(opcodes, ip);
}

void checker(Server* server)
{
	server->clientChecks();
}

void saveWorld(Server* server)
{
	server->saveWorld();
}

void createNPC(Server* server)
{
	server->createAI();
}

void gameLoop()
{
	std::vector<std::string> opcodes;
	std::vector<Client> client;

	char buffer[BUFFER_SIZE];
	int bytes;
	int tempo;
	unsigned int nThreads = std::thread::hardware_concurrency();

	Server* server = new Server();

	SOCKET serverRCV = server->getSocket();
	struct timeval read_timeout = server->getTimeVal();
	SOCKADDR_IN ipep = server->getIpep();

	//std::thread npcThread(createNPC, server);

	std::chrono::system_clock systemClock;
	std::chrono::system_clock::time_point lastRunChecker = systemClock.now();
	std::chrono::system_clock::time_point lastRunSaveWorld = systemClock.now();

	while (1)
	{
		fd_set rfds;

		FD_ZERO(&rfds);
		FD_SET(serverRCV, &rfds);
		int recVal = select(serverRCV + 1, &rfds, NULL, NULL, &read_timeout);

		if (recVal > 0) {
			size_t pos = 0;

			tempo = sizeof(ipep);
			bytes = recvfrom(serverRCV, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&ipep, &tempo);
			std::string ip = inet_ntoa(ipep.sin_addr);

			std::string port = std::to_string(ntohs(ipep.sin_port));

			if (bytes >= 0)
			{
				buffer[bytes - 1] = 0;
				std::string line = buffer;
				//line = decipherPacket(line, server->salt);
				while ((pos = line.find(delimiter)) != std::string::npos)
				{
					std::string token = line.substr(0, line.find(delimiter));
					token += ':' + ip + ':' + port;

					std::cout << token << std::endl;

					if (!token.empty())
						opcodes.push_back(token);
					line.erase(0, pos + delimiter.length());
					token.clear();
				}
				line.clear();
			}

			std::thread thread(runOpcodes, server, opcodes, ip);
			thread.detach();

			memset(buffer, 0, BUFFER_SIZE);
			opcodes.clear();
		}

		if (systemClock.now() - lastRunChecker >= std::chrono::seconds(1))
		{
			lastRunChecker += std::chrono::seconds(2);
			std::thread threadChecker(checker, server);
			threadChecker.detach();
		}

		if (systemClock.now() - lastRunSaveWorld >= std::chrono::seconds(59))
		{
			lastRunSaveWorld += std::chrono::seconds(59);
			std::thread threadSaveWorld(saveWorld, server);
			threadSaveWorld.detach();
		}
	}
}

int main(int argc, char* argv[])
{
	gameLoop();

	return 0;
}