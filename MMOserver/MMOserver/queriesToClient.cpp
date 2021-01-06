#include "Client.h"

void Client::queryClientPosition()
{
	clientWrite("C_GETCURRENTPOSITION");
}