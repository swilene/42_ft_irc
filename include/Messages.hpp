/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:08:18 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/29 19:35:45 by saguesse         ###   ########.fr       */
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

#define WELCOME(nick, user) (":127.0.0.1 001 " + nick + " Welcome to the Internet Relay Network " + nick + "!" + user + "@127.0.0.1\r\n") 

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

		void sendMsg(std::string msg, Client *client);
		void registerMsg(Client *client);
};

#endif
