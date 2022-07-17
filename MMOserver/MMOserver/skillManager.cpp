#include "skillManager.h"

skillManager::skillManager()
{
	xml_document<> doc;
	xml_node<>* root_node = NULL;

	std::ifstream theFile("datasheets/skills.xml");
	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	doc.parse<0>(&buffer[0]);
	root_node = doc.first_node("skillData");

	int i = 0;

	for (xml_node<>* student_node = root_node->first_node("sorcerer"); student_node; student_node = student_node->next_sibling())
	{
		for (xml_node<>* student_node = root_node->first_node("skillTemplate"); student_node; student_node = student_node->next_sibling())
		{
			i++;
		}
	}

	std::cout << "itemsManager initialized: " << i << " item(s) loaded" << std::endl;
}