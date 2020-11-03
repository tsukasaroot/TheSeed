#ifndef SQLMANAGER_H_
#define SQLMANAGER_H_

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <vector>
#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <errno.h>
#include <winsock.h>
#include <algorithm>
#include <map>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

std::vector<std::string> formatString(std::string line);

class SQLManager {
public:
	SQLManager();
	
	void insert(std::string table, std::string column, std::vector<std::string> values);
	void update(std::string user, std::string table, std::vector<std::pair<std::string, std::string>> values);

	std::map<std::string, std::string> checkLogin(std::string nickName);
	std::map<std::string, std::string> initPlayer(std::string name);
private:
	void initialize();

	const char *server = "tcp://127.0.0.1:3306/theseed";
	const char *userName = "root";
	const char *password = "";
	const int port = 3306;
	sql::Driver *driver;
	sql::Connection *con;
	sql::ConnectOptionsMap connection_properties;
};

#endif
