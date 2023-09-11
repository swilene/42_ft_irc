#include "Messages.hpp"

void	Messages::noticeMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)client;
	(void)msg;
	(void)clients;
	(void)channels;

	if (msg[7] == '#') {
		std::string chan = msg.substr(8, msg.find(' ', 7) - 8);

		for (size_t i = 0; i < channels.size(); i++) {
			if (channels[i].getName() == lowercase(chan)) {
				_RPL[NOTICE(client->getNick(), msg)] = channels[i].getMembers();
				return;
			}
		}
	}
	else {
		std::string target = msg.substr(7, msg.find(' ', 7) - 7);

		for (size_t i = 0; i < clients.size(); i++) {
			if (lowercase(clients[i]->getNick()) == lowercase(target)) {
				_RPL[NOTICE(client->getNick(), msg)].push_back(clients[i]);
				return;
			}
		}
	}
}
