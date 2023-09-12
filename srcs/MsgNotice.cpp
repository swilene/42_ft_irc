#include "Messages.hpp"

void	Messages::noticeMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	if (msg[7] == '#') {
		std::string chan = msg.substr(8, msg.find(' ', 7) - 8);

		for (size_t i = 0; i < channels.size(); i++) {
			if (channels[i].getName() == lowercase(chan)) {
				std::string rpl = NOTICE(client->getNick(), client->getUser(), msg);
				_RPL[rpl] = channels[i].getMembers();
				
				//remove sender
				for (size_t i = 0; _RPL[rpl].size(); i++) {
					if (_RPL[rpl][i] == client) {
						_RPL[rpl].erase(_RPL[rpl].begin() + i);
						return;
					}
				}
			}
		}
	}
	else {
		std::string target = msg.substr(7, msg.find(' ', 7) - 7);

		for (size_t i = 0; i < clients.size(); i++) {
			if (lowercase(clients[i]->getNick()) == lowercase(target)) {
				_RPL[NOTICE(client->getNick(), client->getUser(), msg)].push_back(clients[i]);
				return;
			}
		}
	}
}
