/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 15:40:07 by saguesse          #+#    #+#             */
/*   Updated: 2023/07/26 17:08:46 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>

class Client
{
	private:
		int _fd;
		std::string _nickname;
		std::string _username;
		struct pollfd *_pfds;
		struct hostent *_info;

	public:
		Client();
		~Client();

		void setNickname(std::string nickname);
		std::string getNickname() const;
		void setUsername(std::string username);
		std::string getUsername() const;

};

#endif
