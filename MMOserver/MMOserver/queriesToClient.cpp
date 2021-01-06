#include "Client.h"

void Client::queryClientPosition()
{
	clientWrite("C_GETCURRENTPOSITION");
}

void Client::queryClientInventory()
{
	clientWrite("C_GETCURRENTCLIENTINVENTORY");
}