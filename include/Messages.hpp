/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:08:18 by saguesse          #+#    #+#             */
/*   Updated: 2023/09/13 16:28:06 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "Client.hpp"
#include "Channel.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <limits>

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cctype>
#include <sys/types.h>
#include <sys/socket.h>

#define RPL_WELCOME(nick, user) (":127.0.0.1 001 " + nick + " Welcome to the Internet Relay Network " + nick + "!" + user + "@127.0.0.1\r\n") 
#define RPL_YOURHOST(nick, server) (":127.0.0.1 002 " + nick + " Your host is " + server + ", running version 1.0\r\n")
#define RPL_CREATED(nick, date) (":127.0.0.1 003 " + nick + " This server was created " + date + "\r\n")
#define RPL_MYINFO(nick) (":127.0.0.1 004 " + nick + " 127.0.0.1 ft_irc 1.0 aAbcCdefFghHiIjkKmnoOPrRsSwxXy itlko\r\n")
#define RPL_ENDOFWHO(nick, channel) (":127.0.0.1 315 " + nick + " " channel + " :End of /WHO list.\r\n")
#define RPL_ENDOFBANLIST(nick, channel) (":127.0.0.1 368 " + nick + " " + channel + " :End of channel ban list\r\n")
#define PONG(server) ("PONG " + server + "\r\n")
#define NICK(nick, user, newnick) (":" + nick + "!" + user + "@127.0.0.1 NICK " + newnick + "\r\n")
#define MODE(nick, user, channel, mode) (":" + nick + "!" + user + "@127.0.0.1 MODE " + channel + " " + mode + "\r\n")
#define JOIN(nick, user, chan) (":" + nick + "!" + user + "@127.0.0.1 JOIN #" + chan + "\r\n")
#define PART(nick, user, chan, partmsg) (":" + nick + "!" + user + "@127.0.0.1 PART #" + chan + partmsg)
#define PRIVMSG(nick, msg) (":" + nick + " " + msg)
#define NOTICE(nick, user, msg) (":" + nick + "!~" + user + "@localhost " + msg)
#define TOPIC(nick, user, chan, topic) (":" + nick + "!" + user + "@127.0.0.1 TOPIC #" + chan + topic)
#define INVITE(nick, user, nick2, chan) (":" + nick + "!" + user + "@127.0.0.1 INVITE " + nick2 + " #" + chan + "\r\n")
#define KICK(nick, user, nick2, chan, comment) (":" + nick + "!" + user + "@127.0.0.1 KICK #" + chan + " " + nick2 + " :" + comment + "\r\n")
#define QUIT(nick, user, quitMsg) (":" + nick + "!" + user + "@127.0.0.1 QUIT" + quitMsg)

#define BAD_MODE(c) ("Unknown mode character " + c + "\r\n")

#define ERR_NOSUCHNICK(target, nick) ("401 127.0.0.1 " + target + " " + nick + " :No such nick/channel\r\n")
#define ERR_NOSUCHCHANNEL(nick, channel) (":127.0.0.1 403 " + nick + " " + channel + "\r\n")
#define ERR_CANNOTSENDTOCHAN(nick, chan) (":127.0.0.1 404 " + nick + " " + chan + " :Cannot send to channel\r\n")
#define ERR_ERRONEUSNICKNAME(nick, newnick) (":127.0.0.1 432 " + nick + " :Erroneus nickname: " + newnick + "\r\n")
#define ERR_NICKNAMEINUSE(nick, newnick) (":127.0.0.1 433 " + nick + " " + newnick + " :Nickname is already in use\r\n")
#define ERR_USERNOTINCHANNEL(nick, channel) (":127.0.0.1 441 " + nick + " " + nick + " #" + channel + " :They aren't on that channel\r\n")
#define ERR_NOTONCHANNEL(nick, chan) (":127.0.0.1 442 " + nick + " #" + chan + " :You're not on that channel\r\n")
#define ERR_USERONCHANNEL(nick, nick2, chan) (":127.0.0.1 443 " + nick + " " + nick2 + " #" + chan + " :is already on channel\r\n")
#define ERR_NEEDMOREPARAMS(nick, cmd) (":127.0.0.1 461 " + nick + " " + cmd + " :Not enough parameters\r\n")
#define ERR_PASSWDMISMATCH(nick) (":127.0.0.1 464 " + nick + " :Password incorrect\r\n")
#define ERR_CHANNELISFULL(nick, chan) (":127.0.0.1 471 " + nick + " #" + chan + " :Cannot join channel (+l)\r\n")
#define ERR_INVITEONLYCHAN(nick, chan) (":127.0.0.1 473 " + nick + " #" + chan + " :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(nick, chan) (":127.0.0.1 475 " + nick + " #" + chan + " :Cannot join channel (+k)\r\n")
#define ERR_NOPRIVILEGES(nick) (":127.0.0.1 481 " + nick + " :Permission Denied - You're not an IRC operator\r\n")
#define ERR_CHANOPRIVSNEEDED(nick, chan) (":127.0.0.1 482 " + nick + " #" + chan + " :You're not channel operator\r\n")
#define ERR_NOOPERHOST(nick) (":127.0.0.1 491 " + nick + " :No Oper block for your host\r\n")

#define RPL_UMODEIS(nick) (":127.0.0.1 221 " + nick + " +i \r\n")
#define RPL_WHOISUSER(nick, nick2, user, realname) (":127.0.0.1 311 " + nick + " " + nick2 + " " + user + " localhost * :" + realname + "\r\n")
#define RPL_CHANNELMODEIS(nick, channel, mode) (":127.0.0.1 324 " + nick + " " + channel + " " + mode + "\r\n")
#define RPL_NOTOPIC(nick, chan) (":127.0.0.1 331 " + nick + " #" + chan + " :No topic is set\r\n")
#define RPL_TOPIC(nick, chan, topic) (":127.0.0.1 332 " + nick + " #" + chan + " :" + topic + "\r\n")
#define RPL_ENDOFNAMES(nick, chan) (":127.0.0.1 366 " + nick + " #" + chan + " :End of NAMES list\r\n")
#define RPL_INVITING(nick, nick2, chan) (":127.0.0.1 341 " + nick + " " + nick2 + " #" + chan + "\r\n")
#define RPL_YOUREOPER(nick) (":127.0.0.1 368 " + nick + " :You are now an IRC operator\r\n")

class Server;

class Messages
{
	private:
		std::string _servername, _version;
		std::map<std::string, std::vector<Client *> >	_RPL;

		std::time_t _now;
		std::string _time;

	public:
		Messages();
		~Messages();

		std::map<std::string, std::vector<Client *> >	getRPL() const;

		void	parseMsg(std::string msg, Client *client, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	sendRPL(Client *client);
		void	registerMsg(std::string msg, Client *client, std::vector<Client *> clients, std::string password);
		void	pingMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	modeMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	joinMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
			void	join2(Client *client, std::vector<Channel> &channels, std::string chan, std::string pw);
		void	partMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
			void	part2(Client *client, std::vector<Channel> &channels, std::string chan, std::string partMsg);
		void	privMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	noticeMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	quitMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	nickMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	topicMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	inviteMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	kickMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
			void	kickParse(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
			void	kickExec(Client *client, std::vector<Client *> clients, std::vector<Channel> &channels, std::vector<std::string> chans, std::string target, std::string comment);
		void	whoMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	whoisMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	dieMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);
		void	operMsg(Client *client, std::string msg, std::vector<Client *> clients, std::vector<Channel> &channels);

		std::string	lowercase(std::string str);
};

#endif
