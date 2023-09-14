/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 14:28:56 by saguesse          #+#    #+#             */
/*   Updated: 2023/09/14 17:28:24 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Messages.hpp"
#include "Channel.hpp"

#include <exception>
#include <vector>
#include <iostream>
#include <fstream>

#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <poll.h>
#include <unistd.h>
#include <signal.h>

#define BACKLOG 10

class Server
{
	private:
		std::string _port;
		std::string _password;

		std::string _user;
		std::string _host;
		std::string _userPassword;

		std::vector<Client *>	_clients;
		std::vector<pollfd>		_pollfdClients;
		std::vector<Channel>	_channels;
		pollfd _pollfdServer;

		int _listener, _newfd;
		addrinfo hints, *ai;
		socklen_t _addrlen;
		sockaddr_storage _remoteaddr;
		
		Messages 	_msg;
		std::string _msgReceived;

	public:
		Server(std::string port, std::string password);
		~Server();

		std::string getMsgReceived() const;
		void 		getListenerSocket();

		void mainLoop();
		void newClient();
		void clientAlreadyExists(int pos);
		void completeRegistration(int pos);
		void deleteClient(int pos);
		void handlePollerr(int pos);

		class getaddrinfoException : public std::exception
		{
			public:
				const char* what() const throw() { return("Error getaddrinfo()"); }
		};

		class socketException : public std::exception
		{
			public:
				const char* what() const throw() { return("Error socket()"); }
		};

		class setsockoptException : public std::exception
		{
			public:
				const char* what() const throw() { return("Error setsockopt()"); }
		};

		class bindException : public std::exception
		{
			public:
				const char* what() const throw() { return("Error bind()"); }
		};

		class listenException : public std::exception
		{
			public:
				const char* what() const throw() { return("Error listen()"); }
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
