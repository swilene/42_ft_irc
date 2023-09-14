/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:03:00 by saguesse          #+#    #+#             */
/*   Updated: 2023/09/14 17:47:01 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Messages.hpp"

Messages::Messages() : _servername("ft_irc")
{
	_now = std::time(0);
	_time = std::ctime(&_now);
}

Messages::~Messages() {}

std::map<std::string, std::vector<Client *> >	Messages::getRPL() const { return _RPL; }

void Messages::parseMsg(std::string msg, Client *client, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	std::string cmd;
	std::string msgs[15] = {"PING", "MODE", "JOIN", "PRIVMSG", "PART", "QUIT", "NICK", "TOPIC", "INVITE", "KICK", "WHO", "WHOIS", "NOTICE", "OPER", "die"};

	if (msg.find(" ") != std::string::npos)
		cmd = msg.substr(0, msg.find(" ", 0));
	else
		cmd = msg.erase(msg.size() - 2, 2);

	void (Messages::*m[15])(Client *, std::string, std::vector<Client *>, std::vector<Channel>&) = {&Messages::pingMsg,
		&Messages::modeMsg, &Messages::joinMsg, &Messages::privMsg, &Messages::partMsg, &Messages::quitMsg,
		&Messages::nickMsg, &Messages::topicMsg, &Messages::inviteMsg, &Messages::kickMsg, &Messages::whoMsg,
		&Messages::whoisMsg, &Messages::noticeMsg, &Messages::operMsg, &Messages::dieMsg};

	for (int i = 0; i < 15; i++) {
		if (msgs[i] == cmd)
			(this->*m[i])(client, msg, clients, channels);
	}
}

void Messages::sendRPL(Client *client)
{
	std::map<std::string, std::vector<Client *> >::iterator	rpl = _RPL.begin();
	
	send(client->getFd(), rpl->first.c_str(), rpl->first.size(), 0);

	rpl->second.erase(rpl->second.begin());
	if (rpl->second.size() == 0)
		_RPL.erase(rpl);
}

void	Messages::registerMsg(std::string msg, Client *client, std::vector<Client *> clients, std::string password)
{
	std::string nick = msg;

	if (client->getUser().empty()) {
		nick = nick.substr(nick.find("\r\nNICK ") + 7, std::string::npos);
		nick = nick.substr(0, nick.find("\r\n"));

		std::string user = msg;
		user = user.substr(user.find("\r\nUSER ") + 7, std::string::npos);
		user = user.substr(0, user.find(" ", 1));
		client->setUser(user);

		std::string realname = msg;
		realname = realname.substr(realname.find("\r\nUSER ") + 7, std::string::npos);
		realname = realname.substr(realname.find(" :") + 2, std::string::npos);
		realname = realname.substr(0, realname.find("\r\n"));
		client->setRealname(realname);

		// Check password
		if (msg.find("PASS ") == std::string::npos) {
			std::string rpl = ERR_PASSWDMISMATCH(nick);
			rpl += "ERROR :Closing Link: 127.0.0.1 (Bad Password)\r\n";

			_RPL[rpl].push_back(client);
			client->setDelete();
			return;
		}

		std::string pass = msg.substr(msg.find("PASS") + 5, std::string::npos);
		pass = pass.substr(0, pass.find("\r\n"));

		if (pass != password) {
			std::string rpl = ERR_PASSWDMISMATCH(nick);
			rpl += "ERROR :Closing Link: 127.0.0.1 (Bad Password)\r\n";

			_RPL[rpl].push_back(client);
			client->setDelete();
			return;
		}
	}
	else {
		nick = nick.substr(nick.find("NICK ") + 5, std::string::npos);
		nick = nick.substr(0, nick.find("\r\n"));
	}

	// Check if nick already taken
	for (size_t i = 0; i < clients.size(); i++) {
		if (lowercase(nick) == lowercase(clients[i]->getNick())) {
			_RPL[ERR_NICKNAMEINUSE(nick, nick)].push_back(client);
			return ;
		}
	}
	client->setNick(nick);

	std::string rpl = RPL_WELCOME(client->getNick(), client->getUser());
	rpl += RPL_YOURHOST(client->getNick(), _servername);
	rpl += RPL_CREATED(client->getNick(), _time);
	rpl += RPL_MYINFO(client->getNick());
	_RPL[rpl].push_back(client);

	client->setRegistered();
}

std::string	Messages::lowercase(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
		str[i] = tolower(str[i]);
	
	return (str);
}
