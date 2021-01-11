#include "Client.h"

void Client::queryClientPosition()
{
	clientWrite("C_SENDCURRENTPOSITION");
}

void Client::queryClientInventory()
{
	clientWrite("C_GETCURRENTCLIENTINVENTORY");
}