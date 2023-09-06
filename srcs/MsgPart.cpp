#include "Messages.hpp"

void	Messages::partMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)clients;

	std::string	partMsg;
	std::vector<std::string> chans;
	// get channels to leave
	while (msg.find('#', 0) != std::string::npos && msg.find('#', 0) < msg.find(':', 0)) {
		msg = msg.substr(msg.find('#', 0) + 1, std::string::npos);

		std::string chan;
		if (msg.find('#', 0) != std::string::npos && msg.find('#', 0) < msg.find(':', 0))  // 2eme condition si '#' dans partmsg // PAS BIEN!!!!!!!!
			chan = msg.substr(0, msg.find(',', 0));
		else if (msg.find(' ', 0) != std::string::npos)  // == if part message
			chan = msg.substr(0, msg.find(' ', 0));
		else
			chan = msg.substr(0, msg.find('\r', 0));
		chans.push_back(chan);
	}
	// get part message
	if (msg.find(':', 0) != std::string::npos)
		partMsg = msg.substr(msg.find(':', 0) - 1, std::string::npos);  // PAS BIEN!!!!!!
	else
		partMsg = "\r\n";  //+nickname ?

	// DOIT tolower() LES CHANS !!

	// leave parsed channels
	for (size_t i = 0; i < chans.size(); i++) {
		size_t j;
		for (j = 0; j < channels.size(); j++)
			if (channels[j].getName() == lowercase(chans[i]))
				break;
		if (j < channels.size()) {
			if (channels[j].isMember(client)) {
				// s'affiche correctement mais close pas la tab
				std::string reply = ":" + client->getNick() + "!" + client->getUser() + "@127.0.0.1" + " PART #" + chans[i] + partMsg;
				_RPL = reply;
				_RPLtarget = channels[j].getMembers();
				channels[j].rmMember(client);
				// if members == 0 delete channel ????
			}
			else
				std::cout << "you are not on that channel" << std::endl;  //handle error
		}
		else
			std::cout << "chan does not exist" << std::endl;  //handle error
	}
}
