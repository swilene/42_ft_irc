#include "Messages.hpp"

void	Messages::topicMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)clients;

	// _RPLtarget.push_back(client);

	std::string chan = msg.substr(6, std::string::npos);
	std::string	topic;
	if (msg.find(' ', 6) != std::string::npos) {
		chan = chan.substr(0, chan.find(' ', 0));
		topic = msg.substr(msg.find(' ', 6), std::string::npos);
	}
	else
		chan = chan.substr(0, chan.find("\r\n", 0));

	if (chan[0] != '#') {
		// _RPL = ERR_NOSUCHCHANNEL(client->getNick(), chan);
		_RPL[ERR_NOSUCHCHANNEL(client->getNick(), chan)].push_back(client);
		return ;
	}
	chan.erase(0, 1);

	for (size_t i = 0; i < channels.size(); i++) {
		if (channels[i].getName() == lowercase(chan)) {
			if (!topic.empty()) {  //== want to change topic
				if (channels[i].isMember(client)) {
					if (channels[i].isOperator(client)) {   // or mode -t
						// _RPL = TOPIC(client->getNick(), client->getUser(), chan, topic);
						// _RPLtarget = channels[i].getMembers();
						_RPL[TOPIC(client->getNick(), client->getUser(), chan, topic)] = channels[i].getMembers();
						channels[i].setTopic(topic.erase(0, 2));
						return ;
					}
					// _RPL = ERR_CHANOPRIVSNEEDED(client->getNick(), channels[i].getName()); return;
					_RPL[ERR_CHANOPRIVSNEEDED(client->getNick(), channels[i].getName())].push_back(client); return;
				}
				// _RPL = ERR_NOTONCHANNEL(client->getNick(), chan); return;
				_RPL[ERR_NOTONCHANNEL(client->getNick(), chan)].push_back(client);
			}
			else {  // want to display topic
				if (!channels[i].getTopic().empty()) {
					// _RPL = RPL_TOPIC(client->getNick(), channels[i].getName(), channels[i].getTopic()); return;
					_RPL[RPL_TOPIC(client->getNick(), channels[i].getName(), channels[i].getTopic())].push_back(client); return;
				}
				// _RPL = RPL_NOTOPIC(client->getNick(), channels[i].getName()); return;
				_RPL[RPL_NOTOPIC(client->getNick(), channels[i].getName())]; return;
			}
		}
	}
	// _RPL = ERR_NOSUCHCHANNEL(client->getNick(), chan);
	_RPL[ERR_NOSUCHCHANNEL(client->getNick(), chan)].push_back(client);
}
