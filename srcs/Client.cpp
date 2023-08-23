/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 16:56:36 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/23 11:03:10 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd) : _fd(fd), _welcomeSent(false) {}

Client::~Client() {}

int Client::getFd() const { return(_fd); }

bool Client::getWelcomeSent() const { return(_welcomeSent); }
void Client::setWelcomeSent() { _welcomeSent = true; }
