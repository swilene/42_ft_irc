/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:03:00 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/29 19:35:30 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Messages.hpp"

Messages::Messages() : _servername("localhost"), _version("1.1")
{
	_start = time(NULL);
}

Messages::~Messages() {}

void Messages::sendMsg(std::string msg, Client *client, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	std::string cmd = msg.substr(0, msg.find(" ", 0));
	std::string msgs[5] = {"PING", "MODE", "JOIN", "PRIVMSG", "QUIT"};

	void (Messages::*m[5])(Client *, std::string, std::vector<Client *>, std::vector<Channel>&) = {&Messages::pingMsg,
		&Messages::modeMsg, &Messages::joinMsg, &Messages::privMsg, &Messages::quitMsg};

	for (int i = 0; i < 5; i++) {
		if (msgs[i] == cmd)
			(this->*m[i])(client, msg, clients, channels);
	}
	// std::cout << std::endl;  ????????????????
}

void Messages::registerMsg(Client *client)
{
	char		buf[256];
	std::string fullbuf;

	while (fullbuf.find("USER", 0) == std::string::npos) {
		ssize_t recvd = recv(client->getFd(), buf, sizeof(buf), 0);
		if (recvd < 0)
			std::cout << "Error recv()" << std::endl;
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

	_rpl = WELCOME(client->getNick(), client->getUser());
	send(client->getFd(), _rpl.c_str(), _rpl.size(), 0);
	std::cout << "[REPLY] " << _rpl << std::endl;

	//essai pour changer de nick, marche po
	// if (client->getFd() > 4)
	// {
	// 	std::string newnick = nick;
	// 	newnick += client->getFd() + 48;
	// 	std::string nickcmd = ":127.0.0.1 NICK " + nick + " " + newnick;
	// 	send(client->getFd(), nickcmd.c_str(), nickcmd.size(), 0);
	// }
}

void	Messages::pingMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)clients;
	(void)channels;

	std::string	pong = "PONG ";
	std::string	server = msg.substr(msg.find("PING ", 0) + 5, msg.find("\r\n", 0));
	pong += server + "\r\n";
	std::cout << pong << std::endl;

	send(client->getFd(), pong.c_str(), pong.size(), 0);
}

void	Messages::modeMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)client;
	(void)msg;
	(void)clients;
	(void)channels;
}

void	Messages::joinMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)clients;

	std::string name = msg.substr(msg.find("#", 0) + 1, std::string::npos);
	name = name.substr(0, name.find("\r\n", 0));
	size_t		i = 0;
	
	while (i < channels.size()) {
		if (channels[i].getName() == name)
			break;
		i++;
	}
	if (i == channels.size() || channels.size() == 0)
	{
		Channel newchan(name, client->getNick());
		channels.push_back(newchan);
	}
	else {
		std::cout << "joined existing chan" << std::endl;
		channels[i].addMember(client->getNick());
	}
}

void	Messages::privMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	if (msg[8] == '#') {  // == channel msg
		std::string chan = msg.substr(msg.find("#", 0) + 1, std::string::npos);  // juste [8] ?
		chan = chan.substr(0, chan.find(":", 0) - 1);

		size_t id = 0;
		while (id < channels.size() && channels[id].getName() != chan)
			id++;
		
		// verifie pas le POLLOUT ...
		msg = ":" + client->getNick() + " " + msg;  // pour nickname correct
		for (size_t i = 0; i < clients.size(); i++) {
			if (channels[id].isMember(clients[i]->getNick()) && clients[i]->getNick() != client->getNick())
				send(clients[i]->getFd(), msg.c_str(), msg.size(), 0);
		}
	}
	else {  // == dm
		std::string target = msg.substr(8, msg.find(' ', 9) - 8);
		msg = ":" + client->getNick() + " " + msg;

		for (size_t i = 0; i < clients.size(); i++) {
			if (clients[i]->getNick() == target) {  // CASE INSENSITIVE!!
				send(clients[i]->getFd(), msg.c_str(), msg.size(), 0);  // verifie pas le POLLOUT !
				return ;
			}
		}
		// HANDLE ERROR (nick doesnt exist)
		std::cout << "dm target not found" << std::endl;
		// std::string err = "401 127.0.0.1 " + client->getNick() + " " + target + " :No such nick/channel";
		// send(client->getFd(), err.c_str(), err.size(), 0);  //BROKEN
	}
}

void	Messages::quitMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)client;
	(void)msg;
	(void)clients;
	(void)channels;
}
