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

void Messages::sendMsg(std::string msg, Client *client)
{
	Messages message;

	std::string msgs[1] = {"register"};

	void (Messages::*m[1])(Client *) = {&Messages::registerMsg};

	for (int i = 0; i < 1; i++) {
		if (msgs[i] == msg)
			(message.*m[i])(client);
	}
}

void Messages::registerMsg(Client *client)
{
	char buf[256];
	if (recv(client->getFd(), buf, sizeof buf, 0) < 0)
		std::cout << "Error recv()" << std::endl;
	std::cout << buf << std::endl;

	std::string nick = buf;
	nick = nick.substr(nick.find("NICK ", 0) + 5, std::string::npos);
	nick = nick.substr(0, nick.find("\r\n", 0));
	//if (client->getFd() > 4)
	//	nick += client->getFd() + 48;
	client->setNick(nick);

	std::string user = buf;
	user = user.substr(user.find("USER ", 0) + 5, std::string::npos);
	user = user.substr(0, user.find(" ", 1));
	client->setUser(user);

	_rpl = WELCOME(client->getNick(), client->getUser());
	send(client->getFd(), _rpl.c_str(), _rpl.size(), 0);
	std::cout << "[REPLY] " << _rpl << std::endl;
}
