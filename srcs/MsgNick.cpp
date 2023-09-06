#include "Messages.hpp"

void Messages::nickMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)channels;

	msg.erase(0, 5);
	msg.erase(msg.size() - 2, 2);
	if (msg[0] == '#' || msg[0] == '&' || msg[0] == '@' || msg[0] == '!' || msg[0] == '%' || msg[0] == '*' || msg[0] == '(' || msg[0] == ')')
	{
		_RPL = ERR_ERRONEUSNICKNAME(client->getNick(), msg);
		std::cout << "err: " << _RPL << std::endl;
	}
	for (size_t i = 0; i < clients.size(); i++) {
		if (client->getNick() != clients[i]->getNick() && lowercase(clients[i]->getNick()) == lowercase(msg))
			_RPL = ERR_NICKNAMEINUSE(client->getNick(), msg);
	}
	if (_RPL.empty()) {
		_RPL = NICK(client->getNick(), client->getUser(), msg);
		client->setNick(msg);
	}
	_RPLtarget.push_back(client);
}
