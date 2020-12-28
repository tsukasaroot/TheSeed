#include "SQLmanager.h"

SQLManager::SQLManager()
{
	connection_properties["hostName"] = server;
	connection_properties["userName"] = userName;
	connection_properties["password"] = password;
	connection_properties["port"] = port;
	connection_properties["OPT_RECONNECT"] = true;

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
	//initialize()
}

void SQLManager::initialize()
{
	connection_properties["hostName"] = server;
	connection_properties["userName"] = userName;
	connection_properties["password"] = password;
	connection_properties["port"] = port;
	connection_properties["OPT_RECONNECT"] = true;

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
	pstmt = con->prepareStatement(query);

	auto i = 1;
	for (std::vector<std::string>::iterator it = values.begin(); it != values.end(); ++it)
	{
		pstmt->setString(1, *it);
		pstmt->execute();
	}
	delete pstmt;
	// TODO to optimize so it run in one batch
}

void SQLManager::update(std::string user, std::string table, std::vector<std::pair<std::string, std::string>> values)
{
	sql::Statement* stmt;
	std::string query = "UPDATE " + table + " SET ";
	std::string condition = " WHERE name LIKE '" + user + "'";

	for (auto it = values.begin(); it != values.end(); it++)
	{
		query += it->first + " = '" + it->second + "'";
		if (it + 1 != values.end())
			query += " AND ";
	}
	query += condition;

	stmt = con->createStatement();
	stmt->executeUpdate(query);
	delete(stmt);
}

void SQLManager::get(std::string table, std::vector<std::string> fields, std::vector<std::string> columnName, std::vector<std::string> where)
{
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
		stmt = con->createStatement();
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

std::map<std::string, std::string> SQLManager::initPlayer(std::string nickName)
{
	sql::Statement *stmt;
	sql::ResultSet *res;
	std::string query = "SELECT * FROM users WHERE name LIKE '" + nickName + "'";
	std::map<std::string, std::string> result;

	try
	{
		stmt = con->createStatement();
		res = stmt->executeQuery(query);

		while (res->next())
		{
			result.insert(std::pair<std::string, std::string>("x", std::to_string(res->getDouble("x"))));
			result.insert(std::pair<std::string, std::string>("y", std::to_string(res->getDouble("y"))));
			result.insert(std::pair<std::string, std::string>("z", std::to_string(res->getDouble("z"))));
			result.insert(std::pair<std::string, std::string>("mp", std::to_string(res->getInt("mp"))));
			result.insert(std::pair<std::string, std::string>("hp", std::to_string(res->getDouble("hp"))));
			result.insert(std::pair<std::string, std::string>("re", std::to_string(res->getInt("re"))));
			result.insert(std::pair<std::string, std::string>("class", std::to_string(res->getInt("class"))));
			result.insert(std::pair<std::string, std::string>("currency", std::to_string(res->getDouble("currency"))));
			result.insert(std::pair<std::string, std::string>("name", res->getString("name")));
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

std::map<std::string, std::string> SQLManager::checkLogin(std::string nickName)
{
	sql::Statement *stmt;
	sql::ResultSet *res;
	std::string query = "SELECT name, password FROM accounts WHERE name LIKE '" + nickName + "'";
	std::map<std::string, std::string> result;

	try
	{
		stmt = con->createStatement();
		res = stmt->executeQuery(query);

		while (res->next())
		{
			result.insert(std::pair<std::string, std::string>("name", res->getString("name")));
			result.insert(std::pair<std::string, std::string>("password", res->getString("password")));
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