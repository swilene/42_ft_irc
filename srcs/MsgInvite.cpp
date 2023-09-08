#include "Messages.hpp"

void	Messages::inviteMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	std::string nick = msg.substr(7, msg.find(' ', 7) - 7);
	std::string chan = msg.substr(msg.find(' ', 7) + 1, std::string::npos);
	chan = chan.substr(0, chan.find("\r\n"));

	// verif nick then channel
	size_t i;
	for (i = 0; i < clients.size(); i++)
		if (lowercase(clients[i]->getNick()) == lowercase(nick))
			break;
	
	if (i == clients.size()) {
		_RPL[ERR_NOSUCHNICK(nick, client->getNick())].push_back(client); return;
	}
	
	if (chan[0] != '#') {
		_RPL[ERR_NOSUCHCHANNEL(client->getNick(), chan)].push_back(client); return;
	}
	chan.erase(0, 1);

	size_t j;
	for (j = 0; j < channels.size(); j++)
		if (channels[j].getName() == lowercase(chan))
			break;
	
	if (j == channels.size()) {
		_RPL[ERR_NOSUCHCHANNEL(client->getNick(), chan)].push_back(client); return;
	}

	if (channels[j].isMember(clients[i])) {
		_RPL[ERR_USERONCHANNEL(client->getNick(), nick, chan)].push_back(client); return;
	}

	if (!channels[j].isOperator(client)) {
		_RPL[ERR_CHANOPRIVSNEEDED(client->getNick(), chan)].push_back(client); return;
	}

	// send invite
	channels[j].addInvited(clients[i]);

	_RPL[RPL_INVITING(client->getNick(), nick, chan)].push_back(client);
	_RPL[INVITE(client->getNick(), client->getUser(), nick, lowercase(chan))].push_back(clients[i]);
}
