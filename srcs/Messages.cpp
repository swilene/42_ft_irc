/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:03:00 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/25 12:53:29 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Messages.hpp"

Messages::Messages() : _servername("localhost"), _version("1.1")
{
	_start = time(NULL);
}

Messages::~Messages() {}

void Messages::sendMsg(std::string msg, std::vector<Client *>::iterator client)
{
	Messages message;

	std::string msgs[1] = {"register"};

	void (Messages::*m[1])(std::vector<Client *>::iterator) = {&Messages::registerMsg};

	for (int i = 0; i < 1; i++) {
		if (msgs[i] == msg)
			(message.*m[i])(client);
	}
}

void Messages::registerMsg(std::vector<Client *>::iterator client)
{
	_rpl = WELCOME;
	send((*client)->getFd(), _rpl.c_str(), _rpl.size(), 0);
	_rpl = YOURHOST(_servername, _version);
	send((*client)->getFd(), _rpl.c_str(), _rpl.size(), 0);

	_now = localtime(&_start);
	char time[20];
	strftime(time, sizeof(time), "%m-%d-%Y %X", _now);
	_rpl = CREATED(static_cast<std::string>(time));
	send((*client)->getFd(), _rpl.c_str(), _rpl.size(), 0);
	_rpl = MYINFO(_servername, _version);
	send((*client)->getFd(), _rpl.c_str(), _rpl.size(), 0);
	_rpl = ISUPPORT;
	send((*client)->getFd(), _rpl.c_str(), _rpl.size(), 0);
}
