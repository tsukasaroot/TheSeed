#ifndef NPCSPAWNER_H_
#define NPCSPAWNER_H_

#include <windows.h>

#include <iostream>
#include <string>
#include <thread>
#include <map>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <rapidxml/rapidxml.hpp>
using namespace rapidxml;

std::vector<char> openXml(const char* path);

class npcSpawner {
public:
	npcSpawner();
private:
};

#endif