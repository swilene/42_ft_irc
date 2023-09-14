#include "Messages.hpp"

void	Messages::whoisMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)channels;

	std::string nick = msg.substr(6, msg.find("\r\n") - 6);

	for (size_t i = 0; i < clients.size(); i++) {
		if (lowercase(nick) == lowercase(clients[i]->getNick())) {
			_RPL[RPL_WHOISUSER(client->getNick(), nick, clients[i]->getUser(), clients[i]->getRealname())].push_back(client);
			return;
		}
	}
}