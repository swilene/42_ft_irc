/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 15:00:31 by saguesse          #+#    #+#             */
/*   Updated: 2023/09/14 17:26:44 by saguesse         ###   ########.fr       */
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
		std::string _user, _nick, _realname;
		int			_fd;
		std::string _bufMsg;
		bool		_registered;
		bool		_userOperator;
		bool		_delete;

	public:
		Client(int fd);
		~Client();

		// setter
		void setUser(std::string user);
		void setNick(std::string nick);
		void setRealname(std::string nick);
		void setRegistered();
		void setUserOperator();
		void setDelete();

		// getter
		std::string getUser() const;
		std::string getNick() const;
		std::string getRealname() const;
		int 		getFd() const;
		bool		getRegistered() const;
		bool		getUserOperator() const;
		bool		getDelete() const;

		std::string getBufmsg() const;
		void addBufmsg(std::string buf);
		void rmBufmsg();

};

#endif
