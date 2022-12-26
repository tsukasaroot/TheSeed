#ifndef SQLMANAGER_H_
#define SQLMANAGER_H_

#include "mysql_connection.h"

#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <vector>
#include <windows.h>
#include <iostream>
#include <string>
#include <errno.h>
#include <algorithm>
#include <map>
#include <thread>
#include <sstream>
#include <filesystem>
#include <rapidxml/rapidxml.hpp>
using namespace rapidxml;

#pragma comment(lib, "ws2_32.lib")

std::vector<char> openXml(const char* path);
typedef std::unordered_map<std::string, std::map<std::string, std::unordered_map<std::string, std::string>>> Slider;
typedef std::unordered_map<std::string, std::string> PlayerSlider;

class SQLManager {
public:
	SQLManager();
	
	void insert(std::string table, std::string column, std::vector<std::string> values);
	void update(std::string user, std::string cond, std::string table, std::vector<std::pair<std::string, std::string>> values);

	std::map<std::string, std::string> checkLogin(std::string token);
	std::map<std::string, std::string> initPlayer(std::string account_id, std::string player_id);
	std::string getInventory(std::string table, std::vector<std::string> fields, std::vector<std::string> columnName, std::vector<std::string> where);
	std::vector<std::map<std::string, std::string>> retrieve_all_chars(int account_id);
	bool is_name_valid(std::string name);
	void registerNewCharacter(std::string id, std::string name, std::string player_class, PlayerSlider sliders);
private:
	std::string database;
	std::string server;
	std::string userName;
	std::string password = "";
	int port = 3306;

	sql::Driver *driver;
	sql::Connection *con;
	sql::ConnectOptionsMap connection_properties;
	std::map<std::string, std::vector<std::string>> config;
};

#endif
