#include "Messages.hpp"

void	Messages::topicMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)clients;

	std::string chan = msg.substr(6, std::string::npos);
	chan = chan.substr(0, chan.find(' ', 0));
	std::string	topic = msg.substr(msg.find(' ', 6), std::string::npos);
	
	if (chan[0] != '#') {
		std::cout << "no such channel error (no #)" << std::endl;
		return ;
	}
	chan.erase(0, 1);

	for (size_t i = 0; i < channels.size(); i++) {
		if (channels[i].getName() == lowercase(chan)) {
			if (channels[i].isOperator(client)) {
				std::string reply = ":" + client->getNick() + "!" + client->getUser() + "@127.0.0.1" + " TOPIC #" + chan + topic;
				_RPL = reply;
				_RPLtarget = channels[i].getMembers();
				channels[i].setTopic(topic.erase(0, 2));
				return ;
			}
			std::cout << "not operator error" << std::endl; return;
		}
	}
	std::cout << "no such channel error" << std::endl;
}
