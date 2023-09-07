#include "Messages.hpp"

void modeLimit(Channel &channel, bool set, std::string mode, size_t *i)
{
	if (set) {
		(*i)++;
		if (mode[*i] == ' ')
			(*i)++;
		//else gerer erreur
		std::string limit;
		while ((mode[*i] >= '0' && mode[*i] <= '9') || (mode[*i] >= 'a' && mode[*i] <= 'z') || (mode[*i] >= 'A' && mode[*i] <= 'Z')) {
			limit += mode[*i];
			(*i)++;
		}
		size_t nb = atoi(limit.c_str());
		if (nb <= 2147483647)
			channel.setUserLimit(nb);
		std::cout << "debug: limit = " << limit << ", nb = " << nb << std::endl;
	}
	else
		channel.setUserLimit(0);
}

void modeInviteOnly(Channel &channel, bool set)
{
	if (set)
		channel.setInviteOnly(true);
	else
		channel.setInviteOnly(false);
}

void modeTopic(Channel &channel, bool set)
{
	if (set)
		channel.setTopicRights(true);
	else
		channel.setTopicRights(false);
}

void parsingModes(Channel &channel, std::string mode)
{
	bool set;
	std::string newMode;
	newMode += mode[0];
	if (newMode == "+")
		set = true;
	else if (newMode == "-")
		set = false;
	for (size_t i = 1; i < mode.size(); i++) {
		if (mode[i] == '+') {
			if (!set)
				newMode += "+";
			set = true;
		}
		else if (mode[i] == '-') {
			if (set)
				newMode += "-";
			set = false;
		}
		else if (mode[i] == 'i') {
			modeInviteOnly(channel, set);
			newMode += "i";
		}
		else if (mode[i] == 't') {
			modeTopic(channel, set);
			newMode += "t";
		}
		else if (mode[i] == 'l') {
			modeLimit(channel, set, mode, &i);
		}
	}
	std::cout << "debug: newMode = [" << newMode << "]" << std::endl;
}

std::string takeChannelModes(Channel &channel)
{
	//mode o n'est pas a verifier
	std::string mode = "+";
	if (channel.getInviteOnly())
		mode += "i";
	if (channel.getTopicRights())
		mode += "t";
	if (!channel.getPassword().empty())
		mode += "k";
	if (channel.getUserLimit()) {
		mode += "l ";
		mode += channel.getUserLimit();
	}
	return (mode);
}

void	Messages::modeMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)clients;
	(void)channels;
	msg.erase(0, 5);
	size_t pos = msg.find(" ", 0);
	std::string name = msg.substr(0, pos);
	std::string mode;
	if (pos != std::string::npos) { //modes
		msg.erase(0, pos + 1);
		msg.erase(msg.size() - 2, 2);
		mode = msg;
	}
	else //pas modes
		name.erase(name.size() - 2, 2);
	if (name[0] != '#') {
		for (size_t i = 0; i < clients.size(); i++) {
			if (clients[i]->getNick() == name) {
				if (mode.empty())
					_RPL = RPL_UMODEIS(name);
				else
					_RPL = MODE(client->getNick(), client->getUser(), name, mode);
				break;
			}
		}
		if (_RPL.empty())
			_RPL = ERR_NOSUCHCHANNEL(client->getNick(), name);
	}
	else if (name[0] == '#') {
		name.erase(0, 1);
		for (size_t i = 0; i < channels.size(); i++) {
			if (channels[i].getName() == name) {
				if (mode.empty()) {
					mode = takeChannelModes(channels[i]);
					_RPL = RPL_CHANNELMODEIS(client->getNick(), "#" + name, mode);
				}
				else {
					parsingModes(channels[i], mode);
				}
			}
		}
	}
	std::cout << "debug: _RPL = " << _RPL;
	_RPLtarget.push_back(client);
}
