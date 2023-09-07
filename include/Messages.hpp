/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:08:18 by saguesse          #+#    #+#             */
/*   Updated: 2023/09/06 13:59:13 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "Client.hpp"
#include "Channel.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <ctime>
#include <cctype>
#include <sys/types.h>
#include <sys/socket.h>

#define WELCOME(nick, user) (":127.0.0.1 001 " + nick + " Welcome to the Internet Relay Network " + nick + "!" + user + "@127.0.0.1\r\n") 

#define NICK(nick, user, newnick) (":" + nick + "!" + user + "@127.0.0.1 NICK " + newnick + "\r\n")

#define ERR_NICKNAMEINUSE(nick, newnick) (":127.0.0.1 433 " + nick + "\r\n")

#define ERR_ERRONEUSNICKNAME(nick, newnick) (":127.0.0.1 432 " + nick + " :Erroneus nickname: " + newnick + "\r\n")

#define JOIN(nick, user, chan) (":" + nick + "!" + user + "@127.0.0.1 JOIN #" + chan + "\r\n")

#define RPL_TOPIC(nick, chan, topic) (":127.0.0.1 332 " + nick + " #" + chan + " :" + topic + "\r\n")

#define RPL_ENDOFNAMES(nick, chan) (":127.0.0.1 366 " + client->getNick() + " #" + chan + " :End of NAMES list\r\n")


class Server;

class Messages
{
	private:
		std::string _servername, _version;
		std::string	_RPL;
		std::vector<Client *>_RPLtarget;

	public:
		Messages();
		~Messages();

		std::string				getRPL() const;
		std::vector<Client *>	getRPLtarget() const;

		void	parseMsg(std::string msg, Client *client, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	sendRPL(Client *client);
		void	registerMsg(Client *client);
		void	pingMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	modeMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	joinMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
			void	join2(Client *client, std::vector<Channel> &channels, std::string chan, std::string pw);
		void	privMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	partMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	quitMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	nickMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	topicMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);

		std::string	lowercase(std::string str);
};

#endif
