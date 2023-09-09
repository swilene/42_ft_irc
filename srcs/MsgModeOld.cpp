#include "Messages.hpp"

std::string modeOperator(Channel &channel, bool set, std::string mode, size_t *i, std::vector<Client *> clients, std::string &_RPL, Messages &msg)
{
	bool err = false;
	std::string user;
	
	(*i)++;
	if (mode[*i] != ' ')
		return (user);
	(*i)++;
	while (mode[*i] && mode[*i] != ' ') {
		user += mode[*i];
		(*i)++;
	}
	size_t j = 0;
	for (; j < clients.size(); j++) {
		if (msg.lowercase(clients[j]->getNick()) == msg.lowercase(user))
			break;
	}
	if (j == clients.size()) {
		_RPL += ERR_NOSUCHNICK(user);
		err = true;
	}
	j = 0;
	for (; j < channel.getMembers().size(); j++) {
		if (msg.lowercase(channel.getMembers()[j]->getNick()) == msg.lowercase(user)) {
			if (set)
				channel.addOperator(channel.getMembers()[j]);
			else
				channel.rmOperator(channel.getMembers()[j]);
		}
	}
	if (j == channel.getMembers().size()) {
		_RPL += ERR_USERNOTINCHANNEL(user, channel.getName());
		err = true;
	}
	if (err)
		user.clear();
	return (user);
}

bool modePassword(Channel &channel, bool set, std::string mode, size_t *i)
{
	bool res = true;

	if (set) {
		(*i)++;
		if (mode[*i] == ' ') {
			(*i)++;
			std::string password;
			while (mode[*i] && mode[*i] != ' ') {
				if (mode[*i] == ',')
					return (false);
				password += mode[*i];
				(*i)++;
			}
			channel.setPassword(password);
		}
	}
	else {
		if (channel.getPassword().empty())
			res = false;
		//channel.getPassword().clear();
	}
	return (res);
}

bool modeLimit(Channel &channel, bool set, std::string mode, size_t *i, std::string &_RPL)
{
	bool res = true;
	std::cout << "debug: avant[" << mode[*i] << "]" << std::endl;
	(*i)++;
	std::cout << "debug: apres[" << mode[*i] << "]" << std::endl;
	if (mode[*i] == ' ')
		(*i)++;
	else {
		while (mode[*i] && mode[*i] != ' ') {
			std::string c(1, mode[*i]);
			_RPL += BAD_MODE(c);
			(*i)++;
		}
		res = false;
	}
	if (set) {
		std::string limit;
		while ((mode[*i] >= '0' && mode[*i] <= '9') || (mode[*i] >= 'a' && mode[*i] <= 'z') || (mode[*i] >= 'A' && mode[*i] <= 'Z')) {
			limit += mode[*i];
			(*i)++;
		}
		size_t nb = atoi(limit.c_str());
		if (nb > 0 && nb <= 2147483647)
			channel.setUserLimit(nb);
	}
	else {
		channel.setUserLimit(0);
		res = true;
	}
	return (res);
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

void parsingModes(Channel &channel, std::string mode, std::string &_RPL, std::vector<Client *> clients, Messages &msg)
{
	bool set;
	std::string newMode, user;
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
			if (modeLimit(channel, set, mode, &i, _RPL))
				newMode += "l";
		}
		else if (mode[i] == 'k') {
			if (modePassword(channel, set, mode, &i))
				newMode += "k";
		}
		else if (mode[i] == 'o') {
			user = modeOperator(channel, set, mode, &i, clients, _RPL, msg);
			if (!user.empty())
				newMode += "o";
		}
		std::cout << "debug: newMode = [" << newMode << "]" << std::endl;
	}
	mode.clear();
	mode = newMode;
	for (size_t i = 0; i < newMode.size(); i++) {
		if (newMode[i] == '+')
			set = true;
		else if (newMode[i] == '-')
			set = false;
		else if (newMode[i] == 'l' && set) {
			mode += " ";
			char nb[11];
			std::sprintf(nb, "%d", channel.getUserLimit());
			mode += nb;
		}
		else if (newMode[i] == 'k') {
			mode += " ";
			mode += channel.getPassword();
			if (!set)
				channel.getPassword().clear();
		}
		else if (newMode[i] == 'o') {
			mode += " ";
			mode += user;
		}
	}
	std::cout << "debug: mode = [" << mode << "]" << std::endl;
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
					if (channels[i].isOperator(client))
						parsingModes(channels[i], mode, _RPL, clients, *this);
					else
						_RPL += ERR_CHANOPRIVSNEEDED(client->getNick(), channels[i].getName());
				}
			}
		}
	}
	std::cout << "debug: _RPL = " << _RPL;
	_RPLtarget.push_back(client);
}
