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
#include <vector>

#include <poll.h>

class Client
{
	private:
		std::string	_nickname, _username;  // + realname ?
		int			_fd;
		bool		_operator;  // diff par channel ??
		bool		_welcomeSent;  // utile ?
		std::vector<std::string>	_channels;

	public:
		Client(int fd);
		~Client();

		int		getFd() const;
		bool	getOperator() const;
		bool	getWelcomeSent() const;
		void	setWelcomeSent();

		void	addChannel(std::string channel);
		void	rmChannel(std::string channel);
		bool	isMember(std::string channel) const;
};

#endif