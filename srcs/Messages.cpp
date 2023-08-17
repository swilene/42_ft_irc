/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:03:00 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/08 12:19:54 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Messages.hpp"

Messages::Messages() {}

Messages::~Messages() {}

void Messages::sendMsg(std::string msg) const
{
	Messages message;

	std::string msgs[1] = {"connection"};

	void (Messages::*m[1])(void) = {&Messages::connectionMsg};

	for (int i = 0; i < 1; i++) {
		if (msgs[i] == msg)
			(message.*m[i])();
	}
}

void Messages::connectionMsg()
{
	//msg 01 a 04
}
