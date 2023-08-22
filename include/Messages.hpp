/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:08:18 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/22 16:08:39 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <iostream>
#include <string>

//#define WELCOME(nick, user, host) ("Welcome to the ft_irc Network, " + nick + "[!" + user + "@" + host + "]") 
#define WELCOME "Welcome to the ft_irc Network, solene[!solene@localhost]" 

//cpp01 ex05

class Messages
{
	private:
		void connectionMsg() const;

	public:
		Messages();
		~Messages();

		std::string welcomeRPL();
};

#endif
