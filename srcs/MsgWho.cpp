#include "Messages.hpp"

void Messages::whoMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void) clients;
	msg.erase(0, 5);
	msg.erase(msg.size() - 2, 2);
	for (size_t i = 0; i < channels.size(); i++) {
		if (channels[i].getName() == msg)
			_RPL[RPL_ENDOFWHO(client->getNick(), "#" + msg)].push_back(client);
	}
}
