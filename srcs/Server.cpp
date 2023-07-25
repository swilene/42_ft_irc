/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 09:47:48 by saguesse          #+#    #+#             */
/*   Updated: 2023/07/25 18:05:25 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstring>
#include <iostream>
#include <unistd.h>

Server::Server(char *port, char *pswd) : _port(port), _pswd(pswd) {}

Server::~Server() {}

void Server::getServerSocket()
{
	int yes = 1;
	struct addrinfo *tmp;

	memset(&_addrServer, 0, sizeof _addrServer);	// make sure the struct is empty
	_addrServer.ai_family = AF_UNSPEC;				// dont't care IPv4 or IPv6
	_addrServer.ai_socktype = SOCK_STREAM;			// TCP stream socket
	_addrServer.ai_flags = AI_PASSIVE;				// fill in my IP for me
	getaddrinfo(NULL, _port, &_addrServer, &_res);
	for (tmp = _res; tmp; tmp = tmp->ai_next) {
		_sockServer = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
		if (_sockServer >= 0) {
			setsockopt(_sockServer, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
			bind(_sockServer, tmp->ai_addr, tmp->ai_addrlen);
			break ;
		}
	}

	freeaddrinfo(_res);

	std::cout << _sockServer << std::endl;
}
