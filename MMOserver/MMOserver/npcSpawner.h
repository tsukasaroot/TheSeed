#ifndef NPCSPAWNER_H_
#define NPCSPAWNER_H_

#include "xmlParser.h"

#include <windows.h>

#include <iostream>
#include <string>
#include <thread>
#include <map>

class npcSpawner {
public:
	npcSpawner();
private:
	xmlParser* reader;
};

#endif