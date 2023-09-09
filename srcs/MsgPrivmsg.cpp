#include "Messages.hpp"

void	Messages::privMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	if (msg[8] == '#') {  // == channel msg
		std::string chan = msg.substr(9, std::string::npos);
		chan = lowercase(chan.substr(0, chan.find(" ", 0)));

		size_t id = 0;
		while (id < channels.size() && channels[id].getName() != chan)
			id++;

		msg = PRIVMSG(client->getNick(), msg);

		if (!channels[id].isMember(client)) {
			_RPL[ERR_CANNOTSENDTOCHAN(client->getNick(), chan)].push_back(client);
			return ;
		}

		_RPL[msg] = channels[id].getMembers();
		for (size_t i = 0; i < _RPL[msg].size(); i++) {
			if (_RPL[msg][i] == client) {
				_RPL[msg].erase(_RPL[msg].begin() + i);
				break ;
			}
		}
		if (_RPL[msg].size() == 0)
			_RPL.clear();  // clear toute la map car pas plusieurs cmd normalement
	}
	else {  // == dm
		std::string target = msg.substr(8, msg.find(' ', 9) - 8);

		for (size_t i = 0; i < clients.size(); i++) {
			if (lowercase(clients[i]->getNick()) == lowercase(target)) {
				_RPL[PRIVMSG(client->getNick(), msg)].push_back(clients[i]);
				return;
			}
		}
		_RPL[ERR_NOSUCHNICK(target, client->getNick())].push_back(client);
	}
}
