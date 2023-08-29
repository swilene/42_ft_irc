/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 15:00:31 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/24 14:53:26 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

#include <poll.h>

class Client
{
	private:
		std::string	_nickname, _username;  // + realname ?
		int			_fd;
		bool		_welcomeSent;

	public:
		Client(int fd);
		~Client();

		int		getFd() const;
		bool	getWelcomeSent() const;
		void	setWelcomeSent();

};

#endif