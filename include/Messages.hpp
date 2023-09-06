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
#include "Channel.hpp"

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
		void	privMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	partMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	quitMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
};

#endif
