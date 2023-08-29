/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 16:56:36 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/25 12:53:45 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <cctype>

Client::Client(int fd) : _fd(fd), _operator(false), _welcomeSent(false) {}

Client::~Client() {}  // close(_fd) ?

int		Client::getFd() const { return(_fd); }

bool	Client::getOperator() const { return(_operator); }

bool	Client::getWelcomeSent() const { return(_welcomeSent); }
void	Client::setWelcomeSent() { _welcomeSent = true; }

void	Client::addChannel(std::string channel)
{
	// message deja parse ?
	for (size_t i = 0; i < channel.size(); i++)
		channel[i] = tolower(channel[i]);
	_channels.push_back(channel);
}

void	Client::rmChannel(std::string channel)
{
	for (size_t i = 0; i < channel.size(); i++)
		channel[i] = tolower(channel[i]);
	
	for (size_t i = 0; i < _channels.size(); i++) {
		if (_channels[i] == channel) {
			_channels.erase(_channels.begin() + i);
			return;  //peut avoir doublons ?
		}
	}
}

bool	Client::isMember(std::string channel) const
{
	for (size_t i = 0; i < channel.size(); i++)
		channel[i] = tolower(channel[i]); // case insensitive

	for (size_t i = 0; i < _channels.size(); i++) {
		if (_channels[i] == channel)
			return true;
	}
	return false;
}
