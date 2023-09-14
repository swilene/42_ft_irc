/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 16:56:36 by saguesse          #+#    #+#             */
/*   Updated: 2023/09/14 17:26:47 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd) : _fd(fd)
{
	_registered = false;
	_userOperator = false;
	_delete = false;
}

Client::~Client() { close(_fd); }

// setter
void	Client::setUser(std::string user) { _user = user; }
void	Client::setNick(std::string nick) { _nick = nick; }
void	Client::setRealname(std::string realname) { _realname = realname; }
void	Client::setRegistered(void)	   { _registered = true; }
void	Client::setUserOperator() { _userOperator = true; }
void	Client::setDelete() { _delete = true; }

// getter
std::string Client::getUser() const { return (_user); }
std::string Client::getNick() const { return (_nick); }
std::string Client::getRealname() const { return (_realname); }
int 		Client::getFd() const { return(_fd); }
bool		Client::getRegistered() const { return _registered; }
bool		Client::getUserOperator() const { return _userOperator; }
bool		Client::getDelete() const { return _delete; }

std::string	Client::getBufmsg() const { return _bufMsg; }

void	Client::addBufmsg(std::string buf) { _bufMsg += buf; }

void	Client::rmBufmsg() { _bufMsg.clear(); }
