#include "Messages.hpp"

void	Messages::quitMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)channels;

	std::string quitMsg = msg.substr(msg.find("QUIT :", 0) + 4, std::string::npos);
	
	_RPL = ":" + client->getNick() + "!" + client->getUser() + "@127.0.0.1 QUIT" + quitMsg;
	_RPLtarget = clients;

	for (size_t i = 0; i < _RPLtarget.size(); i++) {
		if (_RPLtarget[i] == client) {
			_RPLtarget.erase(_RPLtarget.begin() + i);
			break;
		}
	}
	if (_RPLtarget.size() == 0)
		_RPL.clear();
}
