/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:03:00 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/23 17:23:48 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Messages.hpp"

Messages::Messages() {}

Messages::~Messages() {}

void Messages::sendMsg(std::string msg, std::vector<Client *>::iterator client)
{
	Messages message;

	std::string msgs[1] = {"register"};

	void (Messages::*m[1])(std::vector<Client *>::iterator) = {&Messages::registerMsg};

	for (int i = 0; i < 1; i++) {
		if (msgs[i] == msg)
			(message.*m[i])(client);
	}
}

void Messages::registerMsg(std::vector<Client *>::iterator client)
{
	//send()
	std::cout << (*client)->getFd() << std::endl;
}
