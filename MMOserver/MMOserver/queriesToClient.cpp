#include "Client.h"

void Client::queryClientPosition()
{
	clientWrite("C_SENDCURRENTPOSITION");
	this->positionQuery++;
}

void Client::queryClientInventory()
{
	clientWrite("C_GETCURRENTCLIENTINVENTORY");
}