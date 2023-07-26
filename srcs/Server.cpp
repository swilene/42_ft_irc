/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 09:47:48 by saguesse          #+#    #+#             */
/*   Updated: 2023/07/26 15:36:02 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

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
	
	if (getaddrinfo(NULL, _port, &_addrServer, &_res))
		throw getaddrinfoException();

	for (tmp = _res; tmp; tmp = tmp->ai_next) {
		_sockServer = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
		if (_sockServer >= 0) {
			setsockopt(_sockServer, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
			if (bind(_sockServer, tmp->ai_addr, tmp->ai_addrlen) < 0)
				close(_sockServer);
			else
				break ;
		}
	}

	freeaddrinfo(_res);

	if (!tmp)
		throw socketException();

	if (listen(_sockServer, 10) < 0)
		throw socketException();
}

void Server::mainLoop()
{
	int fd_count = 1;
	struct pollfd pfds[2];

	pfds[0].fd = _sockServer;
	pfds[0].events = POLLIN;

	while(1)
	{
		if (poll(pfds, 2, -1) < 0)
			throw pollException();
		for (int i = 0; i < fd_count; i++) {
			if (pfds[i].revents && POLLIN) {
				if (pfds[i].fd == _sockServer) {
					_addrlen = sizeof _addrClient;
					_sockClient = accept(_sockServer, (struct sockaddr *)&_addrClient, &_addrlen);
					if (_sockClient < 0)
						std::cout << "Error accept()" << std::endl;
					else {
						pfds[1].fd = _sockClient;
						pfds[1].events = POLLIN;
						std::cout << "new connection on socket " << _sockClient << std::endl;
					}
				}
				/*else {
					
				}*/
			}
		}
	}
	std::cout << _sockServer << std::endl;
}
