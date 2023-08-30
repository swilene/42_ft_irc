/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 14:30:00 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/29 19:34:00 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

extern bool exitServer;

Server::Server(std::string port, std::string password) : _port(port), _password(password) {}

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
		throw socketException();
	if (setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
		throw setsockoptException();
	if (bind(_listener, ai->ai_addr, ai->ai_addrlen) < 0) {
		close(_listener);
		throw bindException();
	}
	if (listen(_listener, BACKLOG) < 0) {
		close(_listener);
		throw listenException();
	}

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
		// for (std::vector<pollfd>::iterator it = _pollfdClients.begin(); it != _pollfdClients.end(); it++) {
		for (size_t i = 0; i < _pollfdClients.size(); i++) {
			if (_pollfdClients[i].revents & POLLIN) {
				if (_pollfdClients[i].fd == _listener)
					newClient();
				else {
					clientAlreadyExists(_pollfdClients[i].fd);
					_msg.sendMsg(_RPLToSend, _clients[i - 1], _clients, _channels);
					_RPLToSend.clear();
				}
			}
			// else if (it->revents & POLLOUT)
			// 	handlePollout(it->fd);
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
	// _RPLToSend = "register";
	// _msg.sendMsg(_RPLToSend, newClient);
	_msg.registerMsg(newClient);  // register direct ?
	// _RPLToSend.clear();

	std::cout << "pollserver: new connection on socket " << _newfd << std::endl;
}

void Server::clientAlreadyExists(int fd)
{
	char buf[256];
	int recvd = recv(fd, buf, sizeof buf, 0);

	if (recvd < 0)
		std::cout << "Error recv()" << std::endl;
	else if (recvd == 0) {
		for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
			if ((*it)->getFd() == fd) {
				delete *it;
				_clients.erase(it);
				std::cout << "Client n" << fd << " disconnected" << std::endl;
				break;
			}
	}
	else {
		buf[recvd] = '\0';
		std::cout << "client n" << fd << ": " << buf << std::endl;
		_RPLToSend = buf;
		// _RPLToSend.clear();
		// std::cout << buf << std::endl;
		// std::string ping = buf;
		// if (ping.find("PING ", 0) != std::string::npos) {
		// 	std::string rep = "PONG " + ping.substr(5, ping.size());
		// 	send(fd, ping.c_str(), ping.size(), 0);
		// 	std::cout << "sent pong to n" << _clients[0]->getFd() << std::endl;
		// }
	}
}

void Server::handlePollout(int fd)
{
	for (size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i]->getFd() == fd) {
			// _msg.sendMsg(_RPLToSend, _clients[i]);
			_RPLToSend.clear();
		}
	}
}
