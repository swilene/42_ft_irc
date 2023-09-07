/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:03:00 by saguesse          #+#    #+#             */
/*   Updated: 2023/09/06 14:08:32 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Messages.hpp"

Messages::Messages() : _servername("localhost"), _version("1.1") {}

Messages::~Messages() {}

std::vector<std::string>	Messages::getRPL() const { return _RPL; }
std::vector<std::vector<Client *>> Messages::getRPLtarget() const { return _RPLtarget; }

void Messages::parseMsg(std::string msg, Client *client, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	std::string cmd = msg.substr(0, msg.find(" ", 0));
	std::string msgs[8] = {"PING", "MODE", "JOIN", "PRIVMSG", "PART", "QUIT", "NICK", "TOPIC"};

	void (Messages::*m[8])(Client *, std::string, std::vector<Client *>, std::vector<Channel>&) = {&Messages::pingMsg,
		&Messages::modeMsg, &Messages::joinMsg, &Messages::privMsg, &Messages::partMsg, &Messages::quitMsg,
		&Messages::nickMsg, &Messages::topicMsg};

	for (int i = 0; i < 8; i++) {
		if (msgs[i] == cmd)
			(this->*m[i])(client, msg, clients, channels);
	}
}

void Messages::sendRPL(Client *client)
{
	send(client->getFd(), _RPL[0].c_str(), _RPL[0].size(), 0);

	std::cout << " SENDING [" << _RPL[0].substr(0, _RPL[0].size() - 2) << "\\r\\n] TO [" << client->getNick() << "]" << std::endl; 
	
	_RPLtarget[0].erase(_RPLtarget[0].begin());
	if (_RPLtarget[0].size() == 0) {
		_RPLtarget.erase(_RPLtarget.begin());
		_RPL.erase(_RPL.begin());
	}
}

void Messages::registerMsg(Client *client)
{
	char		buf[256];
	std::string fullbuf;

	while (fullbuf.find("USER", 0) == std::string::npos) {
		ssize_t recvd = recv(client->getFd(), buf, sizeof(buf), 0);
		if (recvd < 0)
			std::cout << "Error recv()" << std::endl; //return ?
		buf[recvd] = '\0';
		fullbuf += buf;
	}

	std::string nick = fullbuf;
	nick = nick.substr(nick.find("NICK ", 0) + 5, std::string::npos);
	nick = nick.substr(0, nick.find("\r\n", 0));
	// tmp, gerer les doublons de NICK
	if (client->getFd() > 4)
		nick += client->getFd() + 48;
	client->setNick(nick);

	std::string user = fullbuf;
	user = user.substr(user.find("USER ", 0) + 5, std::string::npos);
	user = user.substr(0, user.find(" ", 1));
	client->setUser(user);

	_RPL.push_back(WELCOME(client->getNick(), client->getUser()));
	_RPLtarget.push_back(std::vector<std::string>(client));
}

std::string	Messages::lowercase(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
		str[i] = tolower(str[i]);
	
	return (str);
}
