#include "Messages.hpp"

void	Messages::pingMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)clients;
	(void)channels;

	std::string	pong = "PONG ";
	std::string	server = msg.substr(msg.find("PING ", 0) + 5, std::string::npos);
	server = server.substr(0, server.find("\r\n", 0));
	pong += server + "\r\n";

	// _RPL = pong;
	// _RPLtarget.push_back(client);
	_RPL[pong].push_back(client);
}
