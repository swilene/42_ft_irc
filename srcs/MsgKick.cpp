#include "Messages.hpp"

void	Messages::kickMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	while (!msg.empty()) {
		kickParse(client, msg.substr(0, msg.find("\r\n") + 2), clients, channels);
		msg.erase(0, msg.find("\r\n") + 2);
	}
}

void	Messages::kickParse(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)client;
	(void)clients;
	(void)channels;

	// parse all target channels
	msg.erase(0, 5);
	std::string argChans = msg.substr(0, msg.find(' '));
	std::vector<std::string> chans;

	while (argChans.find(',') != std::string::npos) {
		chans.push_back(argChans.substr(0, argChans.find(',')));  // from [1] ?
		argChans = argChans.substr(argChans.find(',') + 1, std::string::npos);
	}
	chans.push_back(argChans);

	// parse target user
	msg.erase(0, msg.find(' ') + 1);
	std::string target = msg.substr(0, msg.find(' '));

	// parse comment
	std::string comment = msg.substr(msg.find(':') + 1, std::string::npos);
	comment.erase(comment.size() - 2, 2);

	kickExec(client, clients, channels, chans, target, comment);


	// std::cout << "CHANNELS: " << std::flush;
	// for (size_t i = 0; i < chans.size(); i++) {
	// 	std::cout << "[" << chans[i] << "] " << std::flush;
	// }
	// std::cout << std::endl;

	// std::cout << "TARGET: " << target << std::endl;
	// if (!comment.empty())
	// 	std::cout << "COMMENT: " << comment << std::endl;
	// else
	// 	std::cout << "COMMENT: [empty]" << std::endl << std::endl;
}

void	Messages::kickExec(Client *client, std::vector<Client *> clients, std::vector<Channel> &channels, std::vector<std::string> chans, std::string target, std::string comment)
{
	// [channel] -> exists ? operator ?  ( only the 1st one ??? )
	// [user]    -> exists ? on channel ?

	chans[0].erase(0, 1);  //remove '#'

	size_t	i;
	for (i = 0; i < channels.size(); i++)
		if (channels[i].getName() == lowercase(chans[0]))
			break;
	
	if (i == channels.size()) {
		_RPL[ERR_NOSUCHCHANNEL(client->getNick(), chans[0])].push_back(client); return;
	}

	if (!channels[i].isMember(client)) {
		_RPL[ERR_NOTONCHANNEL(client->getNick(), chans[0])].push_back(client); return;
	}

	if (!channels[i].isOperator(client)) {
		_RPL[ERR_CHANOPRIVSNEEDED(client->getNick(), chans[0])].push_back(client); return;
	}

	size_t	j;
	for (j = 0; j < clients.size(); j++)
		if (lowercase(clients[j]->getNick()) == lowercase(target))
			break;
	
	if (j == clients.size()) {
		_RPL[ERR_NOSUCHNICK(target, client->getNick())].push_back(client); return;
	}

	if (!channels[i].isMember(clients[j])) {
		_RPL[ERR_USERNOTINCHANNEL(target, chans[0])].push_back(client); return; //target or client[j] nick ?
	}

	if (comment.empty())
		comment = clients[j]->getNick();

	// kick target
	_RPL[KICK(client->getNick(), client->getUser(), clients[j]->getNick(), channels[i].getName(), comment)] = channels[i].getMembers();
	channels[i].rmMember(clients[j]);
}
