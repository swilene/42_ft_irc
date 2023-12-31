#include "Messages.hpp"

std::string modeOperator(Channel &channel, std::vector<std::string> args, size_t &j, std::string &newMode, bool set, std::vector<Client *> clients, std::string &rpl, Messages msg)
{
	bool err = false;
	std::string user;
	
	if (j >= args.size())
		return (user);

	user = args[j];
	j++;

	size_t i = 0;
	for (; i < clients.size(); i++) {
		if (msg.lowercase(clients[i]->getNick()) == msg.lowercase(user))
			break;
	}
	if (i == clients.size()) {
		rpl += ERR_NOSUCHNICK(user, user);
		err = true;
	}
	i = 0;
	for (; i < channel.getMembers().size(); i++) {
		if (msg.lowercase(channel.getMembers()[i]->getNick()) == msg.lowercase(user)) {
			if (set)
				channel.addOperator(channel.getMembers()[i]);
			else
				channel.rmOperator(channel.getMembers()[i]);
			break;
		}
	}
	if (i == channel.getMembers().size()) {
		rpl += ERR_USERNOTINCHANNEL(user, channel.getName());
		err = true;
	}
	if (err)
		user.clear();
	else
		newMode += "o";
	return (user);
}

void modePassword(Channel &channel, std::vector<std::string> args, size_t &j, std::string &newMode, bool set)
{
	std::string password;

	if (j < args.size()) {
		password = args[j];
		j++;
	}
	if (set && !password.empty()) {
		channel.setPassword(password);
		newMode += "k";
	}
	else if (!set) {
		if (!channel.getPassword().empty())
			newMode += "k";
	}
}

void modeLimit(Client *client, Channel &channel, std::vector<std::string> args, size_t &j, std::string &newMode, bool set, std::string &rpl)
{
	if (!set) {
		channel.setUserLimit(0);
		newMode += "l";
	}
	else if (set && j < args.size()) {
		std::string limit;
		size_t i = 0;
		while (args[j][i] && args[j][i] >= '0' && args[j][i] <= '9') {
			limit += args[j][i];
			i++;
		}
		size_t nb = atoi(limit.c_str());
		if (nb > 0 && nb <= 2147483647) {
			channel.setUserLimit(nb);
			newMode += "l";
		}
		j++;
	}
	else
		rpl += ERR_NEEDMOREPARAMS(client->getNick(), "MODE +l");
}

void modeInviteOnly(Channel &channel, bool set, std::string &newMode)
{
	if (set)
		channel.setInviteOnly(true);
	else
		channel.setInviteOnly(false);
	newMode += "i";
}

void modeTopic(Channel &channel, bool set, std::string &newMode)
{
	if (set)
		channel.setTopicRights(true);
	else
		channel.setTopicRights(false);
	newMode += "t";
}

std::string parsingModes(Channel &channel, std::string mode, std::string &rpl, std::vector<Client *> clients, Messages &msg, Client *client)
{
	// separer les commandes des arguments dans 2 vectors
	std::vector<char> modes;
	std::vector<std::string> args;
	size_t i = 0;
	size_t pos;

	while (mode[i] && mode[i] != ' ') {
		modes.push_back(mode[i]);
		i++;
	}

	if (mode[i]) {
		i++;
		while (mode[i]) {
			pos = mode.find(" ", i);
			args.push_back(mode.substr(i, pos - i));
			if (pos != std::string::npos)
				i = pos + 1;
			else
				i = pos;
		}
	}

	// parser les commandes
	bool set; //+: true, -: false
	size_t j = 0; //move in args
	std::string user;
	std::string newMode(1, modes[0]);
	if (newMode == "+")
		set = true;
	else if (newMode == "-")
		set = false;
	for (size_t i = 1; i < modes.size(); i++) {
		if (modes[i] == '+') {
			newMode += "+";
			set = true;
		}
		else if (modes[i] == '-') {
			newMode += "-";
			set = false;
		}
		else if (modes[i] == 'i')
			modeInviteOnly(channel, set, newMode);
		else if (modes[i] == 't')
			modeTopic(channel, set, newMode);
		else if (mode[i] == 'l')
			modeLimit(client, channel, args, j, newMode, set, rpl);
		else if (mode[i] == 'k')
			modePassword(channel, args, j, newMode, set);
		else if (mode[i] == 'o')
			user = modeOperator(channel, args, j, newMode, set, clients, rpl, msg);
		else if (mode[i] != ' ') {
			std::string c(1, mode[i]);
			rpl += BAD_MODE(c);
		}
	}

	if (newMode[newMode.size() - 1] == '-' || newMode[newMode.size() - 1] == '+')
		newMode.erase(newMode.size() - 1, 1);

	// remplir le mode a renvoyer
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
	if (!mode.empty())
		return MODE(client->getNick(), client->getUser(), "#" + channel.getName(), mode);
	return std::string();
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
	if (channel.getUserLimit())
		mode += "l";
	if (!channel.getPassword().empty())
		mode += " " + channel.getPassword();
	if (channel.getUserLimit()) {
		mode += " ";
		char nb[11];
		std::sprintf(nb, "%d", channel.getUserLimit());
		mode += nb;
	}
	return (mode);
}

void	Messages::modeMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	std::string	rpl;
	std::string rplChannel;

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
			if (lowercase(clients[i]->getNick()) == lowercase(name)) {
				if (mode.empty())
					rpl = RPL_UMODEIS(name);
				else
					rpl = MODE(client->getNick(), client->getUser(), name, mode);
				break;
			}
		}
		if (rpl.empty())
			rpl = ERR_NOSUCHCHANNEL(client->getNick(), name);
	}
	else if (name[0] == '#') {
		name.erase(0, 1);
		for (size_t i = 0; i < channels.size(); i++) {
			if (lowercase(channels[i].getName()) == lowercase(name)) {
				if (mode == "b")
					rpl = RPL_ENDOFBANLIST(client->getNick(), "#" + name);
				else if (mode.empty()) {
					mode = takeChannelModes(channels[i]);
					rpl = RPL_CHANNELMODEIS(client->getNick(), "#" + name, mode);
				}
				else {
					if (channels[i].isOperator(client)) {
						rplChannel = parsingModes(channels[i], mode, rpl, clients, *this, client);
						if (!rplChannel.empty())
							_RPL[rplChannel] = channels[i].getMembers();
					}
					else
						rpl += ERR_CHANOPRIVSNEEDED(client->getNick(), channels[i].getName());
				}
			}
		}
	}
	if (!rpl.empty())
		_RPL[rpl].push_back(client);
}
