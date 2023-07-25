/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 09:47:52 by saguesse          #+#    #+#             */
/*   Updated: 2023/07/25 12:48:08 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

class Server
{
	private:
		int _sockServer, _sockClient;
		char *_port, *_pswd;
		char _buf[256];
		struct addrinfo _addrServer, *_res;
		struct sockaddr_storage _addrClient;
		socklen_t _addrlen;

	public:
		Server(char *port, char *pswd);
		~Server();

		void getServerSocket();

		//getters
};
