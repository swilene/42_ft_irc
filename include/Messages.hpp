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

#define WELCOME(nick, user) (":127.0.0.1 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@127.0.0.1\r\n") 
#define PONG(server) ("PONG " + server + "\r\n")
#define NICK(nick, user, newnick) (":" + nick + "!" + user + "@127.0.0.1 NICK " + newnick + "\r\n")
#define MODE(nick, user, channel, mode) (":" + nick + "!" + user + "@127.0.0.1 MODE " + channel + " " + mode + "\r\n")
#define JOIN(nick, user, chan) (":" + nick + "!" + user + "@127.0.0.1 JOIN #" + chan + "\r\n")
#define PART(nick, user, chan, partmsg) (":" + nick + "!" + user + "@127.0.0.1 PART #" + chan + partmsg)
#define PRIVMSG(nick, msg) (":" + nick + " " + msg)
#define TOPIC(nick, user, chan, topic) (":" + nick + "!" + user + "@127.0.0.1 TOPIC #" + chan + topic)
#define QUIT(nick, user, quitMsg) (":" + nick + "!" + user + "@127.0.0.1 QUIT" + quitMsg)

#define ERR_NOSUCHNICK(target, nick) ("401 127.0.0.1 " + target + " " + nick + " :No such nick/channel\r\n")
#define ERR_NOSUCHCHANNEL(nick, channel) (":127.0.0.1 403 " + nick + " " + channel + "\r\n")
#define ERR_CANNOTSENDTOCHAN(nick, chan) (":127.0.0.1 404 " + nick + " " + chan + " :Cannot send to channel\r\n")
#define ERR_ERRONEUSNICKNAME(nick, newnick) (":127.0.0.1 432 " + nick + " :Erroneus nickname: " + newnick + "\r\n")
#define ERR_NICKNAMEINUSE(nick, newnick) (":127.0.0.1 433 " + nick + " " + newnick + " :Nickname is already in use\r\n")
#define ERR_NOTONCHANNEL(nick, chan) (":127.0.0.1 442 " + nick + " #" + chan + " :You're not on that channel\r\n")
#define ERR_INVITEONLYCHAN(nick, chan) (":127.0.0.1 473 " + nick + " #" + chan + " :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(nick, chan) (":127.0.0.1 475 " + nick + " #" + chan + " :Cannot join channel (+k)\r\n")
#define ERR_CHANOPRIVSNEEDED(nick, chan) (":127.0.0.1 482 " + nick + " #" + chan + " :You're not channel operator\r\n")

#define RPL_NOTOPIC(nick, chan) (":127.0.0.1 331 " + nick + " #" + chan + " :No topic is set\r\n")
#define RPL_TOPIC(nick, chan, topic) (":127.0.0.1 332 " + nick + " #" + chan + " :" + topic + "\r\n")
#define RPL_ENDOFNAMES(nick, chan) (":127.0.0.1 366 " + client->getNick() + " #" + chan + " :End of NAMES list\r\n")


class Server;

class Messages
{
	private:
		std::string _servername, _version;
		std::map<std::string, std::vector<Client *> >	_RPL;

	public:
		Messages();
		~Messages();

		std::map<std::string, std::vector<Client *> >	getRPL() const;

		void	parseMsg(std::string msg, Client *client, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	sendRPL(Client *client);
		void	registerMsg(Client *client);
		void	pingMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	modeMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	joinMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
			void	join2(Client *client, std::vector<Channel> &channels, std::string chan, std::string pw);
		void	partMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
			void	part2(Client *client, std::vector<Channel> &channels, std::string chan, std::string partMsg);
		void	privMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	quitMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	nickMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	topicMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);

		std::string	lowercase(std::string str);
};

#endif
