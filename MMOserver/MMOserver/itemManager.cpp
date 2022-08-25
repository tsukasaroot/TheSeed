#include "itemManager.h"

itemsManager::itemsManager()
{
	xml_document<> doc;
	auto buffer = openXml("datasheets/items.xml");
	xml_node<>* root_node = NULL;

	doc.parse<0>(&buffer[0]);
	root_node = doc.first_node("itemData");

	int i = 0;

	for (xml_node<>* student_node = root_node->first_node("ItemTemplate"); student_node; student_node = student_node->next_sibling())
	{
		i++;
	}

	std::cout << "itemsManager initialized: " << i << " item(s) loaded" << std::endl;
}