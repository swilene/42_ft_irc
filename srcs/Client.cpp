/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 16:56:36 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/29 19:37:50 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd) : _fd(fd) { _registered = false; }

Client::~Client() { close(_fd); }

// setter
void Client::setUser(std::string user) { _user = user; }
void Client::setNick(std::string nick) { _nick = nick; }
void Client::setRealname(std::string realname) { _realname = realname; }
void Client::setHost(std::string host) { _host = host; }
void Client::setRegistered(void)	   { _registered = true; }

// getter
std::string Client::getUser() const { return (_user); }
std::string Client::getNick() const { return (_nick); }
std::string Client::getRealname() const { return (_realname); }
std::string Client::getHost() const { return (_host); }
int 		Client::getFd() const { return(_fd); }
bool		Client::getRegistered() const { return _registered; }
std::string	Client::getBufmsg() const { return _bufMsg; }
#include <iostream>
void	Client::addBufmsg(std::string buf) { _bufMsg += buf; }

void	Client::rmBufmsg() { _bufMsg.clear(); }
