#include "SQLmanager.h"

SQLManager::SQLManager()
{
	xml_document<> doc;
	auto buffer = openXml("config/DatabaseConfig.xml");
	xml_node<>* root_node = NULL;

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


/**
* Create a new character for a given account
* TODO: dynamic base stats to register to DB
*/
void SQLManager::registerNewCharacter(std::string id, std::string name, std::string player_class, PlayerSlider sliders)
{
	sql::ResultSet* res;
	std::string query;

	query = "INSERT INTO users (account_id, name, class) VALUES (" + id + ", " + '"' + name + '"' +  ", " + player_class + ")";

	try
	{
		this->con->createStatement()->execute(query);
	}
	catch (sql::SQLException& e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}

	query = "SELECT LAST_INSERT_ID() AS player_id";
	double player_id = NULL;

	try
	{
		res = this->con->createStatement()->executeQuery(query);
		if (res->rowsCount() > 0)
		{
			while (res->next())
			{
				player_id = res->getInt64("player_id");
			}
		}
	}
	catch (sql::SQLException& e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}


	if (player_id != NULL)
	{
		query = "INSERT INTO appearance VALUES (" + std::to_string(player_id) + ", " +
			sliders["head"] + ", " + sliders["body"] + ", " + sliders["feets"] + ", " + sliders["hair"] + ", " + sliders["legs"] + ", " + sliders["race"] + ")";
	}

	try
	{
		this->con->createStatement()->execute(query);
	}
	catch (sql::SQLException& e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}

	query = "INSERT INTO currentplayerstats VALUES (" + std::to_string(player_id) + ", 100, 100, 10, 0, 0, 10, 2000)";
	try
	{
		this->con->createStatement()->execute(query);
	}
	catch (sql::SQLException& e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}

}

bool SQLManager::is_name_valid(std::string name)
{
	std::string query = "SELECT name FROM users WHERE name ='" + name + "'";
	sql::ResultSet* res;

	try {
		res = this->con->createStatement()->executeQuery(query);

		if (res->rowsCount() == 0)
		{
			return true;
		}
	}
	catch (sql::SQLException& e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	return false;
}

std::vector<std::map<std::string, std::string>> SQLManager::retrieve_all_chars(int account_id)
{
	std::string query = "SELECT player_id, name, class, level, lastEnterWorld FROM users WHERE account_id=" + std::to_string(account_id);
	sql::ResultSet* res;
	std::vector<std::map<std::string, std::string>> character_list;

	try {
		res = this->con->createStatement()->executeQuery(query);
		while (res->next()) // columns player_id name class lastEnterWorld
		{
			std::map<std::string, std::string> characters;

			characters.insert(std::pair<std::string, std::string>("player_id", std::to_string(res->getInt64("player_id"))));
			characters.insert(std::pair<std::string, std::string>("name", res->getString("name")));
			characters.insert(std::pair<std::string, std::string>("class", std::to_string(res->getInt("class"))));
			characters.insert(std::pair<std::string, std::string>("lastEnterWorld", res->getString("lastEnterWorld")));
			characters.insert(std::pair<std::string, std::string>("level", std::to_string(res->getInt("level"))));

			character_list.push_back(characters);
		}

		delete res;
	}
	catch (sql::SQLException& e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		exit(0);
	}
	return character_list;
}

std::string SQLManager::getInventory(std::string table, std::vector<std::string> fields, std::vector<std::string> columnName, std::vector<std::string> where)
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
	try
	{
		stmt = this->con->createStatement();
		res = stmt->executeQuery(query);
		std::map<int, std::string> inventories;
		std::string items_id;
		
		while (res->next())
		{
			items_id = res->getString("items_id");
		}
		return items_id;
		delete res;
		delete stmt;
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