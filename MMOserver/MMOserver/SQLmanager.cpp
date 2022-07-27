#include "SQLmanager.h"

SQLManager::SQLManager()
{
	const char* path = "config/DatabaseConfig.xml";
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
	root_node = doc.first_node("mySQL");
	bool reconnector;

	if (xml_node<>* node = root_node->first_node("database"))
	{
		this->database = node->first_attribute("name")->value();
		this->server = node->first_attribute("host")->value();
		this->userName = node->first_attribute("user")->value();
		this->password = node->first_attribute("pass")->value();
		this->port = std::stoi(node->first_attribute("port")->value());
		std::istringstream(node->first_attribute("reconnect")->value()) >> std::boolalpha >> reconnector;
	}

	connection_properties["hostName"] = "tcp://" + this->server + "/" + this->database;
	connection_properties["userName"] = this->userName;
	connection_properties["password"] = this->password;
	connection_properties["OPT_RECONNECT"] = reconnector;

	try
	{
		driver = get_driver_instance();
		this->con = driver->connect(connection_properties);
		std::cout << "SQLManager initialiazed" << std::endl;
	}
	catch (sql::SQLException e)
	{
		std::cout << "Could not connect to server. Error message: " << e.what() << std::endl;
		system("pause");
		exit(1);
	}
}

void SQLManager::insert(std::string table, std::string column, std::vector<std::string> values)
{
	sql::PreparedStatement* pstmt;
	std::string query = "INSERT INTO " + table + "(" + column + ") VALUES(?)";
	pstmt = this->con->prepareStatement(query);

	auto i = 1;
	for (std::vector<std::string>::iterator it = values.begin(); it != values.end(); ++it)
	{
		pstmt->setString(1, *it);
		pstmt->execute();
	}
	delete pstmt;
	// TODO to optimize so it run in one batch
}

void SQLManager::update(std::string user, std::string cond, std::string table, std::vector<std::pair<std::string, std::string>> values)
{
	sql::Statement* stmt;
	std::string query = "UPDATE " + table + " SET ";
	std::string condition = " WHERE " + cond + " = '" + user + "'";

	for (auto it = values.begin(); it != values.end(); it++)
	{
		query += it->first + " = '" + it->second + "'";
		if (it + 1 != values.end())
			query += ", ";
	}
	query += condition;

	stmt = this->con->createStatement();
	stmt->executeUpdate(query);
	delete stmt;
}

void SQLManager::get(std::string table, std::vector<std::string> fields, std::vector<std::string> columnName, std::vector<std::string> where)
{
	// to refactor

	sql::Statement* stmt;
	sql::ResultSet* res;
	std::string query = "SELECT ";
	std::string condition = " WHERE ";

	for (auto it = fields.begin(); it != fields.end(); it++)
	{
		query += *it;
		if (it + 1 != fields.end())
			query += ',';
	}
	query += " FROM " + table + condition;
	
	for (size_t i = 0; i < where.size() && i < columnName.size(); i++)
	{
		query += columnName[i] + "=" + where[i];
		if (i + 1 != where.size() && i + 1 != columnName.size())
			query += " AND ";
	}
	std::cout << query << std::endl;
	try
	{
		stmt = this->con->createStatement();
		res = stmt->executeQuery(query);
		std::map<int, std::vector<double>> inventories;
		std::vector<double> items_id;
		
		while (res->next())
		{
			items_id.push_back(res->getDouble("item_id"));
			inventories.insert(std::pair<int, std::vector<double>>(res->getInt("player_id"), items_id));
		}
		items_id.clear();
		delete res;
		delete stmt;
		for (auto it = inventories.begin(); it != inventories.end(); it++)
		{
			std::cout << "player " << it->first << std::endl;
			for (auto itI = it->second.begin(); itI != it->second.end(); itI++)
			{
				std::cout << "Has those items in inventory: " << *itI << std::endl;
			}
		}
		return;
	}
	catch (sql::SQLException& e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		exit(0);
	}
}

std::map<std::string, std::string> SQLManager::initPlayer(std::string account_id, std::string player_id)
{
	sql::Statement *stmt;
	sql::ResultSet *res;
	std::string query = "SELECT * FROM users WHERE account_id ='" + account_id + "' AND player_id = '" + player_id + "'";
	std::map<std::string, std::string> result;

	try
	{
		stmt = this->con->createStatement();
		res = stmt->executeQuery(query);

		while (res->next())
		{
			result.insert(std::pair<std::string, std::string>("x", std::to_string(res->getDouble("x"))));
			result.insert(std::pair<std::string, std::string>("y", std::to_string(res->getDouble("y"))));
			result.insert(std::pair<std::string, std::string>("z", std::to_string(res->getDouble("z"))));
			result.insert(std::pair<std::string, std::string>("class", std::to_string(res->getInt("class"))));
			result.insert(std::pair<std::string, std::string>("region", std::to_string(res->getInt("region"))));
			result.insert(std::pair<std::string, std::string>("level", std::to_string(res->getInt("level"))));
			result.insert(std::pair<std::string, std::string>("currency", std::to_string(res->getDouble("currency"))));
			result.insert(std::pair<std::string, std::string>("exp", std::to_string(res->getDouble("exp"))));
			result.insert(std::pair<std::string, std::string>("isAlive", std::to_string(res->getBoolean("isAlive"))));
			result.insert(std::pair<std::string, std::string>("player_id", std::to_string(res->getInt("player_id"))));
			result.insert(std::pair<std::string, std::string>("name", res->getString("name")));
		}

		delete res;
		delete stmt;

		sql::Statement* stmt;
		sql::ResultSet* res;
		std::string query = "SELECT * FROM currentplayerstats WHERE player_id = '" + result["player_id"] + "'";

		stmt = con->createStatement();
		res = stmt->executeQuery(query);
		while (res->next())
		{
			result.insert(std::pair<std::string, std::string>("hp", std::to_string(res->getDouble("hp"))));
			result.insert(std::pair<std::string, std::string>("mp", std::to_string(res->getDouble("mp"))));
			result.insert(std::pair<std::string, std::string>("attack", std::to_string(res->getDouble("attack"))));
			result.insert(std::pair<std::string, std::string>("critRate", std::to_string(res->getDouble("critRate"))));
			result.insert(std::pair<std::string, std::string>("critP", std::to_string(res->getDouble("critP"))));
			result.insert(std::pair<std::string, std::string>("defense", std::to_string(res->getDouble("defense"))));
			result.insert(std::pair<std::string, std::string>("re", std::to_string(res->getDouble("re"))));
		}

		delete res;
		delete stmt;

		return result;
	}
	catch (sql::SQLException &e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		exit(0);
	}
}

std::map<std::string, std::string> SQLManager::checkLogin(std::string token)
{
	sql::Statement *stmt;
	sql::ResultSet *res;
	std::string query = "SELECT VIP, name, account_id FROM accounts WHERE token = '" + token + "'";
	std::map<std::string, std::string> result;

	try
	{
		stmt = this->con->createStatement();
		res = stmt->executeQuery(query);

		while (res->next())
		{
			result.insert(std::pair<std::string, std::string>("VIP", res->getString("VIP")));
			result.insert(std::pair<std::string, std::string>("name", res->getString("name")));
			result.insert(std::pair<std::string, std::string>("account_id", res->getString("account_id")));
		}

		delete res;
		delete stmt;

		return result;
	}
	catch (sql::SQLException &e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		exit(0);
	}
}