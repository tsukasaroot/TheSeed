#ifndef SKILLMANAGER_H_
#define SKILLMANAGER_H_

#include <windows.h>

#include <fstream>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <map>
#include <vector>
#include <time.h>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <rapidxml/rapidxml.hpp>
using namespace rapidxml;

std::vector<char> openXml(const char* path);

class skillManager
{
public:
	skillManager();
	~skillManager() {};
private:
};

#endif