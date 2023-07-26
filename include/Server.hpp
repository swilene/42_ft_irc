/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 09:47:52 by saguesse          #+#    #+#             */
/*   Updated: 2023/07/26 16:44:26 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <exception>
#include <list>
#include <poll.h>

class Server
{
	private:
		int _sockServer, _sockClient;
		char *_port, *_pswd;
		char _buf[256];
		struct addrinfo _addrServer, *_res;
		struct sockaddr_storage _addrClient;
		socklen_t _addrlen;
		std::list<Client *> _clients;

	public:
		Server(char *port, char *pswd);
		~Server();

		void getServerSocket();
		void mainLoop();
		void newClient();

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

		class pollException : public std::exception
		{
			public:
				const char* what() const throw() { return("Error poll()"); }
		};
};

#endif
