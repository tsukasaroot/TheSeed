#include "skillManager.h"

skillManager::skillManager()
{
	this->reader = new xmlParser("skills.xml");

	skills = stockXML(this->reader);
	std::cout << "skillManager initialized: " << this->skills.size() << " skill(s) loaded" << std::endl;
}