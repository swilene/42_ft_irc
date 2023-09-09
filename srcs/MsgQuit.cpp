#include "Messages.hpp"

void	Messages::quitMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)channels;

	std::string quitMsg = msg.substr(msg.find("QUIT :", 0) + 4, std::string::npos);

	std::string rpl = QUIT(client->getNick(), client->getUser(), quitMsg);
	_RPL[rpl] = clients;

	// remove leaving user
	for (size_t i = 0; i < _RPL[rpl].size(); i++) {
		if (_RPL[rpl][i] == client) {
			_RPL[rpl].erase(_RPL[rpl].begin() + i);
			break ;
		}
	}
	if (_RPL[rpl].size() == 0)
		_RPL.clear();
}
