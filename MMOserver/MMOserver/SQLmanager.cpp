#include "SQLmanager.h"

SQLManager::SQLManager()
{
	initialize();
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
	sql::PreparedStatement *pstmt;
	std::string query = "INSERT INTO " + table + "(" + column + ") VALUES(?)";
	pstmt = con->prepareStatement(query);

	auto i = 1;
	for (std::vector<std::string>::iterator it = values.begin(); it != values.end(); ++it) {
		pstmt->setString(1, *it);
		pstmt->execute();
	}
	delete pstmt;
	// TODO to optimize so it run in one batch
}

std::map<std::string, std::string> SQLManager::checkLogin(std::string nickName)
{
	sql::Statement *stmt;
	sql::ResultSet *res;
	std::string query = "SELECT name, password, x, y, z FROM users WHERE name LIKE '" + nickName + "'";
	std::map<std::string, std::string> result;

	try
	{
		stmt = con->createStatement();
		res = stmt->executeQuery(query);

		while (res->next()) {
			std::string name = res->getString("name");
			std::string pass = res->getString("password");
			std::string x = std::to_string(res->getDouble("x"));
			std::string y = std::to_string(res->getDouble("y"));
			std::string z = std::to_string(res->getDouble("z"));

			result.insert(std::pair<std::string, std::string>("name", name));
			result.insert(std::pair<std::string, std::string>("password", pass));
			result.insert(std::pair<std::string, std::string>("x", x));
			result.insert(std::pair<std::string, std::string>("y", y));
			result.insert(std::pair<std::string, std::string>("z", z));
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
	}
}