#include "Messages.hpp"

void	Messages::joinMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)clients;

	// PARSE ALL CHANNELS + PASSWORDS IN MAP
	std::string chans = msg.substr(5, msg.find(' ', 6) - 5);
	std::string passwords;
	if (chans.find("\r\n", 0) == std::string::npos) {  // == passwords
		passwords = msg.substr(msg.find(' ', 6) + 1, std::string::npos);
		passwords.erase(passwords.size() - 2, 2);
	}
	else {
		chans.erase(chans.size() - 2, 2);
	}

	std::map<std::string, std::string>	elems;

	while (chans.find(',', 0) != std::string::npos) {
		std::string key = chans.substr(1, chans.find(',', 0) - 1);
		std::string value;
		if (!passwords.empty())
			value = passwords.substr(0, passwords.find(',', 0));
		elems[key] = value;

		chans = chans.substr(chans.find(',', 0) + 1, std::string::npos);
		if (!passwords.empty())
			passwords = passwords.substr(passwords.find(',', 0) + 1, std::string::npos);
	}
	elems[chans.erase(0, 1)] = passwords;  //last elem

	for (std::map<std::string, std::string>::iterator it = elems.begin(); it != elems.end(); it++)
		join2(client, channels, it->first, it->second);
}

void	Messages::join2(Client *client, std::vector<Channel> &channels, std::string chan, std::string pw)
{
	(void)pw; // a faire

	size_t		i = 0;
	std::string	mainRpl;  // full RPL for new member

	while (i < channels.size()) {
		if (channels[i].getName() == lowercase(chan))
			break;
		i++;
	}
	if (i == channels.size() || channels.size() == 0) {
		Channel newchan(lowercase(chan), client);
		channels.push_back(newchan);
		mainRpl = JOIN(client->getNick(), client->getUser(), chan);
	}
	else {  //check for pw and invite-only
		if (channels[i].getInviteOnly() == true) {  // && NOT INVITED) // Check avant ou apres PW ?
			_RPL[ERR_INVITEONLYCHAN(client->getNick(), chan)].push_back(client); return;   // PAS TESTER
		}
		// MUST SAVE INVITE LIST AS Client*  (nick change)
		// IF JOIN AFTER INVITE -> REMOVE FROM INVITE LIST
		if (channels[i].getPassword().empty() || channels[i].getPassword() == pw) {
			channels[i].addMember(client);
			_RPL[JOIN(client->getNick(), client->getUser(), chan)] = channels[i].getMembers();
		}
		else {  // wrong password
			_RPL[ERR_BADCHANNELKEY(client->getNick(), chan)].push_back(client); return;  // PAS TESTER
		}
	}
	// RPL_TOPIC
	if (!channels[i].getTopic().empty())
		mainRpl += RPL_TOPIC(client->getNick(), chan, channels[i].getTopic());
	// RPL_NAMREPLY
	mainRpl += ":127.0.0.1 353 " + client->getNick() + " = #" + chan + " :";
	for (size_t j = 0; j < channels[i].getMembers().size(); j++) {
		if (channels[i].isOperator(channels[i].getMembers()[j]))  //==is ope
			mainRpl += "@";
		mainRpl += channels[i].getMembers()[j]->getNick() + " ";
	}
	mainRpl += "\r\n";
	// RPL_ENDOFNAMES
	mainRpl += RPL_ENDOFNAMES(client->getNick(), chan);
	_RPL[mainRpl].push_back(client);
}
