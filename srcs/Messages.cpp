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

Messages::Messages() : _servername("localhost"), _version("1.1") {}

Messages::~Messages() {}

std::string	Messages::getRPL() const { return _RPL; }
std::vector<Client *> Messages::getRPLtarget() const { return _RPLtarget; }

void Messages::parseMsg(std::string msg, Client *client, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	std::string cmd = msg.substr(0, msg.find(" ", 0));
	std::string msgs[7] = {"PING", "MODE", "JOIN", "PRIVMSG", "PART", "QUIT", "NICK"};

	void (Messages::*m[7])(Client *, std::string, std::vector<Client *>, std::vector<Channel>&) = {&Messages::pingMsg,
		&Messages::modeMsg, &Messages::joinMsg, &Messages::privMsg, &Messages::partMsg, &Messages::quitMsg, &Messages::nickMsg};

	for (int i = 0; i < 7; i++) {
		if (msgs[i] == cmd)
			(this->*m[i])(client, msg, clients, channels);
	}
}

void Messages::sendRPL(Client *client)
{
	send(client->getFd(), _RPL.c_str(), _RPL.size(), 0);

	std::cout << " SENDING [" << _RPL.substr(0, _RPL.size() - 2) << "\\r\\n] TO [" << client->getNick() << "]" << std::endl; 
	
	_RPLtarget.erase(_RPLtarget.begin());
	if (_RPLtarget.size() == 0)
		_RPL.clear();
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

	_RPL = WELCOME(client->getNick(), client->getUser());
	_RPLtarget.push_back(client);
}

void	Messages::pingMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)clients;
	(void)channels;

	std::string	pong = "PONG ";
	std::string	server = msg.substr(msg.find("PING ", 0) + 5, std::string::npos);
	server = server.substr(0, server.find("\r\n", 0));
	pong += server + "\r\n";

	_RPL = pong;
	_RPLtarget.push_back(client);
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
		if (channels[i].getName() == lowercase(name))
			break;
		i++;
	}
	if (i == channels.size() || channels.size() == 0)
	{
		Channel newchan(lowercase(name), client);
		channels.push_back(newchan);
		_RPL = ":" + client->getNick() + "!" + client->getUser() + "@127.0.0.1" + " JOIN #" + name + "\r\n";
		_RPLtarget.push_back(client);
	}
	else {
		std::cout << "joined existing chan" << std::endl;
		channels[i].addMember(client);
		_RPL = ":" + client->getNick() + "!" + client->getUser() + "@127.0.0.1" + " JOIN #" + name + "\r\n";
		_RPLtarget.push_back(client);
	}
}

void	Messages::privMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	if (msg[8] == '#') {  // == channel msg
		std::string chan = msg.substr(msg.find("#", 0) + 1, std::string::npos);  // juste [8] ?
		chan = lowercase(chan.substr(0, chan.find(":", 0) - 1));

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

void	Messages::partMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)clients;

	std::string	partMsg;
	std::vector<std::string> chans;
	// get channels to leave
	while (msg.find('#', 0) != std::string::npos && msg.find('#', 0) < msg.find(':', 0)) {
		msg = msg.substr(msg.find('#', 0) + 1, std::string::npos);

		std::string chan;
		if (msg.find('#', 0) != std::string::npos && msg.find('#', 0) < msg.find(':', 0))  // 2eme condition si '#' dans partmsg
			chan = msg.substr(0, msg.find(',', 0));
		else if (msg.find(' ', 0) != std::string::npos)  // == if part message
			chan = msg.substr(0, msg.find(' ', 0));
		else
			chan = msg.substr(0, msg.find('\r', 0));
		chans.push_back(chan);
	}
	// get part message
	if (msg.find(':', 0) != std::string::npos)
		partMsg = msg.substr(msg.find(':', 0) - 1, std::string::npos);
	else
		partMsg = "\r\n";  //+nickname ?

	// DOIT tolower() LES CHANS !!

	// leave parsed channels
	for (size_t i = 0; i < chans.size(); i++) {
		size_t j;
		for (j = 0; j < channels.size(); j++)
			if (channels[j].getName() == lowercase(chans[i]))
				break;
		if (j < channels.size()) {
			if (channels[j].isMember(client)) {
				// s'affiche correctement mais close pas la tab
				std::string reply = ":" + client->getNick() + "!" + client->getUser() + "@127.0.0.1" + " PART #" + chans[i] + partMsg;
				_RPL = reply;
				_RPLtarget = channels[j].getMembers();
				channels[j].rmMember(client);
				// if members == 0 delete channel ????
			}
			else
				std::cout << "you are not on that channel" << std::endl;  //handle error
		}
		else
			std::cout << "chan does not exist" << std::endl;  //handle error
	}
}

void	Messages::quitMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)channels;

	std::string quitMsg = msg.substr(msg.find("QUIT :", 0) + 4, std::string::npos);
	
	_RPL = ":" + client->getNick() + "!" + client->getUser() + "@127.0.0.1 QUIT" + quitMsg;
	_RPLtarget = clients;

	for (size_t i = 0; i < _RPLtarget.size(); i++) {
		if (_RPLtarget[i] == client) {
			_RPLtarget.erase(_RPLtarget.begin() + i);
			break;
		}
	}
	if (_RPLtarget.size() == 0)
		_RPL.clear();
}

void Messages::nickMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels)
{
	(void)channels;

	msg.erase(0, 5);
	msg.erase(msg.size() - 2, 2);
	if (msg[0] == '#' || msg[0] == '&' || msg[0] == '@' || msg[0] == '!' || msg[0] == '%' || msg[0] == '*' || msg[0] == '(' || msg[0] == ')')
	{
		_RPL = ERR_ERRONEUSNICKNAME(client->getNick(), msg);
		std::cout << "err: " << _RPL << std::endl;
	}
	for (size_t i = 0; i < clients.size(); i++) {
		if (client->getNick() != clients[i]->getNick() && lowercase(clients[i]->getNick()) == lowercase(msg))
			_RPL = ERR_NICKNAMEINUSE(client->getNick(), msg);
	}
	if (_RPL.empty()) {
		_RPL = NICK(client->getNick(), client->getUser(), msg);
		client->setNick(msg);
	}
	_RPLtarget.push_back(client);
}

std::string	Messages::lowercase(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
		str[i] = tolower(str[i]);
	
	return (str);
}
