#include "Messages.hpp"

void	Messages::privMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	if (msg[8] == '#') {  // == channel msg
		std::string chan = msg.substr(msg.find("#", 0) + 1, std::string::npos);  // juste [8] ?
		chan = lowercase(chan.substr(0, chan.find(":", 0) - 1));   // ':' PAS BIEN!!!!!

		size_t id = 0;
		while (id < channels.size() && channels[id].getName() != chan)
			id++;

		msg = ":" + client->getNick() + " " + msg;  // pour nickname correct

		if (!channels[id].isMember(client)) {
			std::cout << "you are not on that channel" << std::endl;
			return ;
		}

		_RPL = msg;
		_RPLtarget = channels[id].getMembers();
		for (size_t i = 0; i < _RPLtarget.size(); i++) {
			if (_RPLtarget[i] == client) {
				_RPLtarget.erase(_RPLtarget.begin() + i);
				break ;
			}
		}
		if (_RPLtarget.size() == 0)
			_RPL.clear();
	}
	else {  // == dm
		std::string target = msg.substr(8, msg.find(' ', 9) - 8);
		msg = ":" + client->getNick() + " " + msg;

		for (size_t i = 0; i < clients.size(); i++) {
			if (lowercase(clients[i]->getNick()) == lowercase(target)) {  // CASE INSENSITIVE!!
				_RPL = msg;
				_RPLtarget.push_back(clients[i]);
				return;
			}
		}
		// HANDLE ERROR (nick doesnt exist)
		std::cout << "dm target not found" << std::endl;
		std::string err = "401 127.0.0.1 " + target + " " + client->getNick() + " :No such nick/channel\r\n";
		_RPL = err;
		_RPLtarget.push_back(client);
	}
}
