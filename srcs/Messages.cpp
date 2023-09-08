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

std::map<std::string, std::vector<Client *> >	Messages::getRPL() const { return _RPL; }

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
	std::map<std::string, std::vector<Client *> >::iterator	rpl = _RPL.begin();
	
	send(client->getFd(), rpl->first.c_str(), rpl->first.size(), 0);

	std::cout << " SENDING [" << rpl->first.substr(0, rpl->first.size() - 2) << "\\r\\n] TO [" << client->getNick() << "]" << std::endl << std::endl; 

	rpl->second.erase(rpl->second.begin());
	if (rpl->second.size() == 0)
		_RPL.erase(rpl);
}

void Messages::registerMsg(Client *client)
{
	char		buf[256];
	std::string fullbuf;

	while (fullbuf.find("USER", 0) == std::string::npos) {
		ssize_t recvd = recv(client->getFd(), buf, sizeof(buf), 0);
		if (recvd < 0)
			std::cout << "Error recv()" << std::endl; //return ? //faudrait throw() qqchose
		buf[recvd] = '\0';
		fullbuf += buf;
	}

	std::string nick = fullbuf;
	nick = nick.substr(nick.find("NICK ", 0) + 5, std::string::npos);
	nick = nick.substr(0, nick.find("\r\n", 0));
	// tmp, gerer les doublons de NICK
	if (client->getFd() > 4) {
		//// A FAIRE, doit envoyer ERR_NICKNAMEINUSE
		// std::string nicktest = ERR_NICKNAMEINUSE(nick, nick + "_");
		// send(client->getFd(), nicktest.c_str(), nicktest.size(), 0);

		// char buf2[256];
		// ssize_t recvd2 = recv(client->getFd(), buf2, sizeof(buf2), 0);
		// buf2[recvd2] = '\0';
		// std::cout << "NEW BUF? = " << buf2 << std::endl;
		// nick += "_";

		nick += client->getFd() + 48;
	}
	client->setNick(nick);

	std::string user = fullbuf;
	user = user.substr(user.find("USER ", 0) + 5, std::string::npos);
	user = user.substr(0, user.find(" ", 1));
	client->setUser(user);

	_RPL[WELCOME(client->getNick(), client->getUser())].push_back(client);
	//// TEST, necessaire ??
	// std::string fullrpl = WELCOME(client->getNick(), client->getUser());
	// fullrpl += ":127.0.0.1 002 " + nick + " :Your host is 127.0.0.1, running version ircd-ratbox-3.0.10\r\n";
	// fullrpl += ":127.0.0.1 003 " + nick + " :This server was created Sun Oct 2 2016 at 04:55:27 CEST\r\n";
	// fullrpl += ":127.0.0.1 004 " + nick + " :127.0.0.1 ircd-ratbox-3.0.10 oiwszcrkfydnxbauglZCD biklmnopstveIrS bkloveI\r\n";

	// _RPL[fullrpl].push_back(client);
}

std::string	Messages::lowercase(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
		str[i] = tolower(str[i]);
	
	return (str);
}
