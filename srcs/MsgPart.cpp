#include "Messages.hpp"

void	Messages::partMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)clients;

	std::string	partMsg;
	std::vector<std::string> chans;
	msg.erase(0,5);

	// get part message
	if (msg.find(' ', 0) != std::string::npos) {
		partMsg = msg.substr(msg.find(' ', 0), std::string::npos);
		msg = msg.substr(0, msg.find(' ', 0));
	}
	else {
		partMsg = "\r\n";
		msg = msg.substr(0, msg.find("\r\n", 0));
	}

	// get channels to leave
	while (msg.find(',', 0) != std::string::npos) {
		chans.push_back(msg.substr(1, msg.find(',', 0) - 1));

		msg = msg.substr(msg.find(',', 0) + 1, std::string::npos);
	}
	chans.push_back(msg.erase(0, 1));

	for (size_t i = 0; i < chans.size(); i++)
		part2(client, channels, chans[i], partMsg);
}

void	Messages::part2(Client *client, std::vector<Channel> &channels, std::string chan, std::string partMsg)
{
	for (size_t i = 0; i < channels.size(); i++) {
		if (channels[i].getName() == lowercase(chan)) {
			if (channels[i].isMember(client)) {
				_RPL[PART(client->getNick(), client->getUser(), chan, partMsg)] = channels[i].getMembers();
				
				channels[i].rmMember(client);
				if (channels[i].getMembers().size() == 0)
					channels.erase(channels.begin() + i);
				return; 
			}
			_RPL[ERR_NOTONCHANNEL(client->getNick(), chan)].push_back(client); return;
		}
	}
	_RPL[ERR_NOSUCHCHANNEL(client->getNick(), chan)].push_back(client); return;
}
