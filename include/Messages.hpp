/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:08:18 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/24 17:25:14 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "Client.hpp"

#include <iostream>
#include <string>
#include <vector>

#include <ctime>
#include <sys/types.h>
#include <sys/socket.h>

//#define WELCOME(nick, user, host) ("Welcome to the ft_irc Network, " + nick + "[!" + user + "@" + host + "]") 
#define WELCOME "Welcome to the Internet Relay Network, solene!solene@localhost\r\n" 
#define YOURHOST(servername, version) ("Your host is 42_ft_irc (" + servername + ") running version " + version + "\r\n")
#define CREATED(time) ("This server was created " + time + "\r\n")
#define MYINFO(servername, version) (servername + " " + version + " io itkol\r\n")
#define ISUPPORT "CHANNELLEN=32 NICKLEN=9 TOPICLEN=307 are supported by this server\r\n"

//cpp01 ex05

class Server;

class Messages
{
	private:
		std::string _rpl, _servername, _version;
		time_t _start;
		tm *_now;

	public:
		Messages();
		~Messages();

		void sendMsg(std::string msg, std::vector<Client *>::iterator client);
		void registerMsg(std::vector<Client *>::iterator client);
};

#endif
