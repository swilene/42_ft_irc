#include "Messages.hpp"

extern bool exitServer;

void Messages::dieMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void) msg;
	(void) clients;
	(void) channels;

	if (client->getUserOperator())
		exitServer = true;
	else
		_RPL[ERR_NOPRIVILEGES(client->getNick())].push_back(client);
}
