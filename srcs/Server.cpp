/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 09:47:48 by saguesse          #+#    #+#             */
/*   Updated: 2023/07/24 18:37:55 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <cstring>
#include <iostream>
#include <unistd.h>

Server::Server(char *port, char *pswd) : _port(port), _pswd(pswd)
{
	memset(&_addrServer, 0, sizeof _addrServer);	// make sure the struct is empty
	_addrServer.ai_family = AF_UNSPEC;				// dont't care IPv4 or IPv6
	_addrServer.ai_socktype = SOCK_STREAM;			// TCP stream socket
	_addrServer.ai_flags = AI_PASSIVE;				// fill in my IP for me
	
	// try catch block to check if all the functions worked
	_status = getaddrinfo(NULL, _port, &_addrServer, &_res);
	_sockServer = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);
	bind(_sockServer, _res->ai_addr, _res->ai_addrlen);
	std::cout << "bind" << std::endl;

	while (1)
	{
		listen(_sockServer, 10);
		std::cout << "listen" << std::endl;
	
		_addr_size = sizeof _addrClient;
		_sockClient = accept(_sockServer, (struct sockaddr *)&_addrClient, &_addr_size);
		std::cout << "accept" << std::endl;
	}

	close(_sockClient);
	close(_sockServer);
}

Server::~Server()
{
	freeaddrinfo(_res);
}
