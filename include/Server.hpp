/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 09:47:52 by saguesse          #+#    #+#             */
/*   Updated: 2023/07/24 18:27:36 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

class Server
{
	private:
		int _status, _sockServer, _sockClient;
		char *_port, *_pswd;
		struct addrinfo _addrServer, *_res;
		struct sockaddr_storage _addrClient;
		socklen_t _addr_size;

	public:
		Server(char *port, char *pswd);
		~Server();

		//getters
};
