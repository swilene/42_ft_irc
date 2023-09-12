#include "Messages.hpp"

void Messages::operMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void) clients;
	(void) channels;
	std::string rpl;

	msg.erase(0, 5);
	msg.erase(msg.size() - 2, 2);

	std::string name, password;
	size_t pos = msg.find(" ");
	name = msg.substr(0, pos);
	msg.erase(0, pos + 1);
	password = msg;
	if (lowercase(name) != lowercase(client->getNick())) {
		_RPL[ERR_NOOPERHOST(client->getNick())].push_back(client);
		return ;
	}

	std::string line;
	std::ifstream file;
	file.open("operator_config", std::ifstream::in);
	if (!file.is_open()) {
		_RPL["Error open() operator_config\r\n"].push_back(client);
		return ;
	}
	
	while (std::getline(file, line)) {
		if (line.find(name) != std::string::npos) {
			if (client->getUserOperator()) {
				_RPL["You are already an operator\r\n"].push_back(client);
				break ;
			}
			if (line.find(password) != std::string::npos) {
				client->setUserOperator();
				_RPL[RPL_YOUREOPER(client->getNick())].push_back(client);
				break ;
			}
			_RPL[ERR_PASSWDMISMATCH(client->getNick())].push_back(client);
			break ;
		}
	}

	if (file.eof())
		_RPL[ERR_NOOPERHOST(client->getNick())].push_back(client);
	
	file.close();
}
