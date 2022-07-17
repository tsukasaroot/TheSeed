#include "itemManager.h"

itemsManager::itemsManager()
{
	xml_document<> doc;
	xml_node<>* root_node = NULL;

	std::ifstream theFile("datasheets/items.xml");
	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	// Parse the buffer
	doc.parse<0>(&buffer[0]);

	// Find out the root node
	root_node = doc.first_node("itemData");

	int i = 0;

	for (xml_node<>* student_node = root_node->first_node("ItemTemplate"); student_node; student_node = student_node->next_sibling())
	{
		i++;
	}

	std::cout << "itemsManager initialized: " << i << " item(s) loaded" << std::endl;
}