/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 15:00:31 by saguesse          #+#    #+#             */
/*   Updated: 2023/09/12 15:32:39 by saguesse         ###   ########.fr       */
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
		std::string _user, _nick, _realname, _host;  // _host necessaire ?
		int			_fd;
		std::string _bufMsg;
		bool		_registered;
		bool		_userOperator;

	public:
		Client(int fd);
		~Client();

		// setter
		void setUser(std::string user);
		void setNick(std::string nick);
		void setRealname(std::string nick);
		void setHost(std::string host);
		void setRegistered();
		void setUserOperator();

		// getter
		std::string getUser() const;
		std::string getNick() const;
		std::string getRealname() const;
		std::string getHost() const;
		int 		getFd() const;
		bool		getRegistered() const;
	  bool		getUserOperator() const;
    std::string getBufmsg() const;

		void addBufmsg(std::string buf);
		void rmBufmsg();

};

#endif
