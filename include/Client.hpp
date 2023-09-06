/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 15:00:31 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/29 19:38:16 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

#include <cctype>
#include <unistd.h> //close
#include <poll.h>

class Client
{
	private:
		std::string _user, _nick, _host;
		int			_fd;
		// pollfd		&_pollfd;  // marche pas comme ca

	public:
		Client(int fd);
		~Client();

		// setter
		void setUser(std::string user);
		void setNick(std::string nick);
		void setHost(std::string host);

		// getter
		std::string getUser() const;
		std::string getNick() const;
		std::string getHost() const;
		int 		getFd() const;
};

#endif