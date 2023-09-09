#include "Messages.hpp"

void	Messages::pingMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)clients;
	(void)channels;

	std::string	server = msg.substr(msg.find("PING ", 0) + 5, std::string::npos);
	server = server.substr(0, server.find("\r\n", 0));

	_RPL[PONG(server)].push_back(client);
}
