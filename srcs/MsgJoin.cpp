#include "Messages.hpp"

void	Messages::joinMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)clients;

	std::string name = msg.substr(msg.find("#", 0) + 1, std::string::npos);
	name = name.substr(0, name.find("\r\n", 0));
	size_t		i = 0;

	while (i < channels.size()) {
		if (channels[i].getName() == lowercase(name))
			break;
		i++;
	}
	if (i == channels.size() || channels.size() == 0)
	{
		Channel newchan(lowercase(name), client);
		channels.push_back(newchan);
		_RPL = ":" + client->getNick() + "!" + client->getUser() + "@127.0.0.1" + " JOIN #" + name + "\r\n";
		_RPLtarget.push_back(client);
	}
	else {
		std::cout << "joined existing chan" << std::endl;
		channels[i].addMember(client);
		_RPL = ":" + client->getNick() + "!" + client->getUser() + "@127.0.0.1" + " JOIN #" + name + "\r\n";
		_RPLtarget.push_back(client);
		if (!channels[i].getTopic().empty())  //RPL_TOPIC
			_RPL += ":127.0.0.1 332 " + client->getNick() + " #" + name + " :" + channels[i].getTopic() + "\r\n";
	}
	// RPL_NAMREPLY
	_RPL += ":127.0.0.1 353 " + client->getNick() + " = #" + name + " :";
	for (size_t j = 0; j < channels[i].getMembers().size(); j++) {
		if (channels[i].isOperator(channels[i].getMembers()[j]))  //==is ope
			_RPL += "@";
		_RPL += channels[i].getMembers()[j]->getNick();  // c moche
		if (j + 1 < channels[i].getMembers().size())
			_RPL += " ";
	}
	_RPL += "\r\n";
	// RPL_ENDOFNAMES
	_RPL += ":127.0.0.1 366 " + client->getNick() + " #" + name + " :End of NAMES list\r\n";
}
