/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 14:28:56 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/23 17:22:41 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Messages.hpp"

#include <exception>
#include <vector>
#include <iostream>
#include <fstream>

#include <cstring>
#include <ctime>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <poll.h>
#include <unistd.h>
#include <signal.h>

#define BACKLOG 10

class Client;
class Messages;

class Server
{
	private:
		std::string _port;
		std::string _password;

		std::string _user;
		std::string _host;
		std::string _userPassword;

		std::vector<Client *> _clients;
		std::vector<pollfd> _pollfdClients;
		std::vector<pollfd> _pollfdNew;
		pollfd _pollfdServer;

		int _listener, _newfd;
		addrinfo hints, *ai;
		socklen_t _addrlen;
		sockaddr_storage _remoteaddr;
		
		Messages _msg();
		std::string _RPLToSend;

	public:
		Server(std::string port, std::string password);
		~Server();

		std::string getRPLToSend() const;
		void getListenerSocket();

		void mainLoop();
		void newClient();
		void clientAlreadyExists(int fd) const;
		void handlePollout(int fd);
		//void handlePollin();

		class openException : public std::exception
		{
			public:
				const char* what() const throw() { return("Error open() user_config"); }
		};

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
