#include "skillManager.h"

skillManager::skillManager()
{
	xml_document<> doc;
	auto buffer = openXml("datasheets/skills.xml");
	xml_node<>* root_node = NULL;

	doc.parse<0>(&buffer[0]);
	root_node = doc.first_node("skillData");

	int i = 0;

	if (xml_node<>* student_node = root_node->first_node("sorcerer"))
	{
		for (xml_node<>* student_node = root_node->first_node("skillTemplate"); student_node; student_node = student_node->next_sibling())
		{
			i++;
		}
	}

	std::cout << "itemsManager initialized: " << i << " item(s) loaded" << std::endl;
}