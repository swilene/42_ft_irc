#include "Messages.hpp"

void	Messages::modeMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)clients;
	(void)channels;
	msg.erase(0, 5);
	std::cout << "debug: msg = " << msg << std::endl;
	size_t pos = msg.find(" ", 0);
	std::string name = msg.substr(0, pos);
	std::string mode;
	if (msg[0] != '#') {
		mode = msg.substr(pos + 1, 2);
		size_t i = 0;
		for (; i < clients.size(); i++) {
			if (clients[i]->getNick() == name) {
				_RPL = MODE(client->getNick(), client->getUser(), name, mode);
				break;
			}
		}
		if (i == clients.size())
			_RPL = ERR_NOSUCHCHANNEL(client->getNick(), name);
	}
	else if (msg[0] == '#') {
		name.erase(0, 1);
		std::cout << "debug: name = [" << name << "]" << std::endl;
		/*for (size_t i = 0; i < channels.size(); i++) {
			if ()
		}*/
		_RPL = MODE(client->getNick(), client->getUser(), name, mode);
	}
	_RPLtarget.push_back(client);
}
