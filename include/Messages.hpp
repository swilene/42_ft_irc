/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:08:18 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/23 17:19:37 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "Server.hpp"

#include <iostream>
#include <string>

//#define WELCOME(nick, user, host) ("Welcome to the ft_irc Network, " + nick + "[!" + user + "@" + host + "]") 
#define WELCOME "Welcome to the Internet Relay Chat Network, solene[!solene@localhost]\r\n" 
#define YOURHOST "Your host is 42_ft_irc, running version 1.1\r\n"
#define CREATED "This server was created"

//cpp01 ex05

class Messages
{
	private:

	public:
		Messages();
		~Messages();

		void sendMsg(std::string msg, std::vector<Client *>::iterator client);
		void registerMsg(std::vector<Client *>::iterator client);
};

#endif
