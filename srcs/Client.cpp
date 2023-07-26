/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 15:41:08 by saguesse          #+#    #+#             */
/*   Updated: 2023/07/26 17:08:31 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() {}

Client::~Client() {}

void Client::setNickname(std::string nickname) { _nickname = nickname; }

std::string Client::getNickname() const { return (_nickname); }

void Client::setUsername(std::string username) { _username = username; }

std::string Client::getUsername() const { return (_username); }
