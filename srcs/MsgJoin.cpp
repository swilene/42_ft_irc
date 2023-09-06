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
	}
}
