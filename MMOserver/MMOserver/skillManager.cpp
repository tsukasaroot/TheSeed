#include "skillManager.h"

skillManager::skillManager()
{
	this->reader = new xmlParser("skills.xml");

	this->skills = stockXML(this->reader);
	for (auto it = this->skills.begin(); it != this->skills.end(); it++)
	{
		std::cout << "Skill name : " << it->first << " with stats ";
		for (auto itS = it->second.begin(); itS != it->second.end(); itS++)
		{
			std::cout << *itS << ';';
		}
		std::cout << std::endl;
	}
	std::cout << "skillManager initialized: " << this->skills.size() << " skill(s) loaded" << std::endl;
}