/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 14:28:56 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/19 15:42:49 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"

#include <exception>
#include <vector>
#include <iostream>

#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <poll.h>
#include <unistd.h>
#include <signal.h>

#define PORT "6667"
#define BACKLOG 10

class Client;

class Server
{
	private:
		std::vector<Client *> _clients;
		std::vector<pollfd> _pollfdClients;
		std::vector<pollfd> _pollfdNew;
		pollfd _pollfdServer;
		int _listener, _newfd;
		addrinfo hints, *ai;
		socklen_t _addrlen;
		sockaddr_storage _remoteaddr;


	public:
		Server();
		~Server();

		void getListenerSocket();
		void mainLoop();
		void newClient();
		void clientAlreadyExists(int fd) const;
		void handlePollout(int fd) const;
		//void handlePollin();

		class getaddrinfoException : public std::exception
		{
			public:
				const char* what() const throw() { return("Error getaddrinfo()"); }
		};

		class socketException : public std::exception
		{
			public:
				const char* what() const throw() { return("Error getting listening socket"); }
		};

		class acceptException : public std::exception
		{
			public:
				const char* what() const throw() { return("Error accept()"); }
		};

		class pollException : public std::exception
		{
			public:
				const char* what() const throw() { return("Error poll()"); }
		};
};

#endif
