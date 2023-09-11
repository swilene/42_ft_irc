/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:03:00 by saguesse          #+#    #+#             */
/*   Updated: 2023/09/11 14:07:18 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Messages.hpp"

Messages::Messages() : _servername("localhost"), _version("1.1") {}

Messages::~Messages() {}

std::map<std::string, std::vector<Client *> >	Messages::getRPL() const { return _RPL; }

void Messages::parseMsg(std::string msg, Client *client, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	std::string cmd = msg.substr(0, msg.find(" "));
	std::string msgs[12] = {"PING", "MODE", "JOIN", "PRIVMSG", "PART", "QUIT", "NICK", "TOPIC", "INVITE", "KICK", "WHO", "WHOIS"};

	void (Messages::*m[12])(Client *, std::string, std::vector<Client *>, std::vector<Channel>&) = {&Messages::pingMsg,
		&Messages::modeMsg, &Messages::joinMsg, &Messages::privMsg, &Messages::partMsg, &Messages::quitMsg,
		&Messages::nickMsg, &Messages::topicMsg, &Messages::inviteMsg, &Messages::kickMsg, &Messages::whoMsg, &Messages::whoisMsg};

	for (int i = 0; i < 12; i++) {
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

void Messages::registerMsg(Client *client, std::vector<Client *> clients, std::vector<pollfd> newpollfd, std::string password)
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
	nick = nick.substr(nick.find("\r\nNICK ") + 7, std::string::npos);
	nick = nick.substr(0, nick.find("\r\n"));

	// Check for password
	if (fullbuf.find("\r\nPASS ") == std::string::npos) {
		std::string rpl = ERR_PASSWDMISMATCH(nick);
		rpl += "ERROR :Closing Link: 127.0.0.1 (Bad Password)\r\n";  // mettre ?

		do { poll(newpollfd.data(), newpollfd.size(), -1); } while (!(newpollfd[0].revents & POLLOUT));
		send(client->getFd(), rpl.c_str(), rpl.size(), 0);
		return;
	}

	std::string pass = fullbuf.substr(fullbuf.find("\r\nPASS") + 7, std::string::npos);
	pass = pass.substr(0, pass.find("\r\n"));

	if (pass != password) {  // case sensitive ??
		std::string rpl = ERR_PASSWDMISMATCH(nick);
		rpl += "ERROR :Closing Link: 127.0.0.1 (Bad Password)\r\n";  // mettre ?

		do { poll(newpollfd.data(), newpollfd.size(), -1); } while (!(newpollfd[0].revents & POLLOUT));
		send(client->getFd(), rpl.c_str(), rpl.size(), 0);
		return;
	}

	// Check if nick already taken
	size_t i;
	do {
		for (i = 0; i < clients.size(); i++) {
			if (lowercase(nick) == lowercase(clients[i]->getNick())) {
				std::string rpl = ERR_NICKNAMEINUSE(nick, nick);

				do { poll(newpollfd.data(), newpollfd.size(), -1); } while (!(newpollfd[0].revents & POLLOUT));
				send(client->getFd(), rpl.c_str(), rpl.size(), 0);

				do { poll(newpollfd.data(), newpollfd.size(), -1); } while (!(newpollfd[0].revents & POLLIN));
				ssize_t recvd = recv(client->getFd(), buf, sizeof(buf), 0);
				buf[recvd] = '\0';

				nick = buf;
				nick = nick.substr(nick.find("NICK ") + 5, std::string::npos);
				nick = nick.substr(0, nick.find("\r\n"));

				break;
			}
		}
	} while (i < clients.size());

	client->setNick(nick);

	std::string user = fullbuf;
	user = user.substr(user.find("\r\nUSER ") + 7, std::string::npos);
	user = user.substr(0, user.find(" ", 1));
	client->setUser(user);

	std::string realname = fullbuf;
	realname = realname.substr(realname.find("\r\nUSER ") + 7, std::string::npos);
	realname = realname.substr(realname.find(" :") + 2, std::string::npos);
	realname = realname.substr(0, realname.find("\r\n"));
	client->setRealname(realname);

	// _RPL[WELCOME(client->getNick(), client->getUser())].push_back(client);
	std::string rpl = RPL_WELCOME(client->getNick(), client->getUser());
	rpl += RPL_MYINFO(client->getNick());
	_RPL[rpl].push_back(client);

	client->setRegistered();

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
