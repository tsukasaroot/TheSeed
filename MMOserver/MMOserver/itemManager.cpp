#include "itemManager.h"

itemsManager::itemsManager()
{
	const char* path = "datasheets/items.xml";
	xml_document<> doc;
	xml_node<>* root_node = NULL;

	if (!std::filesystem::exists(path))
	{
		std::cerr << "File " << path << " not found" << std::endl;
		exit(-1);
	}

	std::ifstream theFile(path);
	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);
	root_node = doc.first_node("itemData");

	int i = 0;

	for (xml_node<>* student_node = root_node->first_node("ItemTemplate"); student_node; student_node = student_node->next_sibling())
	{
		i++;
	}

	std::cout << "itemsManager initialized: " << i << " item(s) loaded" << std::endl;
}