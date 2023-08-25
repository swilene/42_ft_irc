/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 14:30:00 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/25 12:53:22 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

extern bool exitServer;

Server::Server(std::string port, std::string password) : _port(port), _password(password)
{
	std::ifstream file;
	std::string line;
	size_t pos;

	file.open("user_config");
	if (file.is_open()) {
		while (std::getline(file, line)) {
			pos = line.find(" ");
			_user = line.substr(0, pos);
			line.erase(0, pos + 1);
			pos = line.find(" ");
			_host = line.substr(0, pos);
			_userPassword = line.substr(pos + 1);
		}
		file.close();
	}
	else
		throw openException();
}

Server::~Server() {}

std::string Server::getRPLToSend() const { return(_RPLToSend); }

void Server::getListenerSocket()
{
	int yes = 1;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, _port.c_str(), &hints, &ai) < 0)
		throw getaddrinfoException();

	_listener = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	if (_listener < 0)
		std::cout << "Error" << std::endl;
	if (setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
		std::cout << "Error" << std::endl;
	if (bind(_listener, ai->ai_addr, ai->ai_addrlen) < 0) {
		std::cout << "Error" << std::endl;
		close(_listener);
	}
	if (listen(_listener, BACKLOG) < 0)
		std::cout << "Error" << std::endl;

	freeaddrinfo(ai);
}

void Server::mainLoop()
{
	_pollfdServer.fd = _listener;
	_pollfdServer.events = POLLIN;
	_pollfdClients.push_back(_pollfdServer);

	while (exitServer == false) {
		if (poll((pollfd *)&_pollfdClients[0], _pollfdClients.size(), -1) < 0)
			throw pollException();
		//run through the existing connections looking for data to read
		for (std::vector<pollfd>::iterator it = _pollfdClients.begin(); it != _pollfdClients.end(); it++) {
			if (it->revents & POLLIN) {
				if (it->fd == _listener)
					newClient();
				else
					clientAlreadyExists(it->fd);
			}
			else if (it->revents & POLLOUT)
				handlePollout(it->fd);
			//else if (it->revents & POLLERR)
		}
		_pollfdClients.insert(_pollfdClients.end(), _pollfdNew.begin(), _pollfdNew.end());
		_pollfdNew.clear();
	}
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		delete *it;
	_clients.clear();
}

void Server::newClient()
{
	_addrlen = sizeof _remoteaddr;
	_newfd = accept(_listener, (sockaddr *)&_remoteaddr, &_addrlen);
	if (_newfd < 0)
		throw acceptException();
	
	pollfd newPollfd;
	newPollfd.fd = _newfd;
	newPollfd.events = POLLIN | POLLOUT;
	_pollfdNew.push_back(newPollfd);
	
	Client *newClient = new Client(_newfd);
	_clients.push_back(newClient);
	_RPLToSend = "register";

	std::cout << "pollserver: new connection on socket " << _newfd << std::endl;
}

void Server::clientAlreadyExists(int fd) const
{
	char buf[256];

	if (recv(fd, buf, sizeof buf, 0) < 0)
		std::cout << "Error recv()" << std::endl;
}

void Server::handlePollout(int fd)
{
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) { 
		if ((*it)->getFd() == fd) {
			_msg.sendMsg(_RPLToSend, it);
			_RPLToSend.clear();
		}
	}
}
