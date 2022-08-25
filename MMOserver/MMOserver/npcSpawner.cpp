#include "npcSpawner.h"

npcSpawner::npcSpawner()
{
	xml_document<> doc;
	auto buffer = openXml("datasheets/npcs.xml");
	xml_node<>* root_node = NULL;

	doc.parse<0>(&buffer[0]);
	root_node = doc.first_node("npcData");

	int i = 0;

	for (xml_node<>* student_node = root_node->first_node("npcTemplate"); student_node; student_node = student_node->next_sibling())
	{
		i++;
	}

	std::cout << "itemsManager initialized: " << i << " item(s) loaded" << std::endl;
}