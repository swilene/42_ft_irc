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

Client::Client(int fd, pollfd &pfd) : _fd(fd), _pollfd(pfd) {}

Client::~Client() { close(_fd); }

// setter
void Client::setUser(std::string user) { _user = user; }
void Client::setNick(std::string nick) { _nick = nick; }
void Client::setHost(std::string host) { _host = host; }

// getter
std::string Client::getUser() const { return (_user); }
std::string Client::getNick() const { return (_nick); }
std::string Client::getHost() const { return (_host); }
pollfd&		Client::getPollfd() const { return (_pollfd); }
int 		Client::getFd() const { return(_fd); }
