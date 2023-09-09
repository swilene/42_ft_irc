#include "Messages.hpp"

void Messages::nickMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)channels;

	msg.erase(0, 5);
	msg.erase(msg.size() - 2, 2);
	if (msg.find_first_of("#&@!%*()") == 0 || msg.find(',') != std::string::npos) {
		_RPL[ERR_ERRONEUSNICKNAME(client->getNick(), msg)].push_back(client);
		return ;
	}
	for (size_t i = 0; i < clients.size(); i++) {
		if (client->getNick() != clients[i]->getNick() && lowercase(clients[i]->getNick()) == lowercase(msg)) {
			_RPL[ERR_NICKNAMEINUSE(client->getNick(), msg)].push_back(client);
			return ;
		}
	}
	_RPL[NICK(client->getNick(), client->getUser(), msg)].push_back(client);
	client->setNick(msg);
}
