/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 15:00:31 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/15 17:21:31 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

#include <poll.h>

class Client
{
	private:
		std::string _username, _nickname;
		pollfd pfds;

	public:
		Client();
		~Client();

		/*void setFd(int fd);
		void setEvents(short events);

		int getFd() const;
		short getEvents() const;
		short getRevents() const;*/
		pollfd  getPfds();
};

#endif
