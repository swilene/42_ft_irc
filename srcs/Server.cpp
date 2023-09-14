/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 14:30:00 by saguesse          #+#    #+#             */
/*   Updated: 2023/09/12 15:26:27 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

extern bool exitServer;

Server::Server(std::string port, std::string password) : _port(port), _password(password) {}

Server::~Server()
{
	for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		delete *it;
}

std::string Server::getMsgReceived() const { return(_msgReceived); }

void	Server::getListenerSocket()
{
	int yes = 1;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, _port.c_str(), &hints, &ai) < 0 && !exitServer)
		throw getaddrinfoException();

	_listener = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	if (_listener < 0 && !exitServer)
		throw socketException();
	if (setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0 && !exitServer)
		throw setsockoptException();
	if (bind(_listener, ai->ai_addr, ai->ai_addrlen) < 0 && !exitServer) {
		close(_listener);
		throw bindException();
	}
	if (listen(_listener, BACKLOG) < 0 && !exitServer) {
		close(_listener);
		throw listenException();
	}

	freeaddrinfo(ai);
}

void	Server::mainLoop()
{
	_pollfdServer.fd = _listener;
	_pollfdServer.events = POLLIN;
	_pollfdClients.push_back(_pollfdServer);

	while (exitServer == false) {
		if (poll(_pollfdClients.data(), _pollfdClients.size(), -1) < 0 && !exitServer)
			throw pollException();
		//if reply to send
		if (!_msg.getRPL().empty()) {
			for (size_t i = 1; i < _pollfdClients.size(); i++) {
				if (_msg.getRPL().begin()->second[0] == _clients[i - 1] && _pollfdClients[i].revents & POLLOUT) {
					_msg.sendRPL(_clients[i - 1]);
					if (_clients[i - 1]->getDelete())
						deleteClient(i - 1);
					break;
				}
			}
		}
		else {
			//run through the existing connections looking for data to read
			for (size_t i = 0; i < _pollfdClients.size(); i++) {
				if (_pollfdClients[i].revents & POLLIN) {
					//if listener connection
					if (_pollfdClients[i].fd == _listener) { newClient(); }
					//if unregistered client
					else if (!_clients[i - 1]->getRegistered()) { completeRegistration(i); }
					//if registered client
					else { clientAlreadyExists(i); }

					if (!_msgReceived.empty()) {
						if (_clients[i - 1]->getRegistered())
							_msg.parseMsg(_msgReceived, _clients[i - 1], _clients, _channels);
						else
							_msg.registerMsg(_msgReceived, _clients[i - 1], _clients, _password);
						_msgReceived.clear();
					}
					break;
				}
				else if (_pollfdClients[i].revents & POLLERR) {
					handlePollerr(i);
					break;
				}
			}
		}
	}
}

void	Server::newClient()
{
	_addrlen = sizeof _remoteaddr;
	_newfd = accept(_listener, (sockaddr *)&_remoteaddr, &_addrlen);
	if (_newfd < 0 && !exitServer)
		throw acceptException();
	
	pollfd newPollfd;
	newPollfd.fd = _newfd;
	newPollfd.events = POLLIN | POLLOUT;
	_pollfdClients.push_back(newPollfd);
	
	Client *newClient = new Client(_newfd);
	_clients.push_back(newClient);
}

void	Server::clientAlreadyExists(int pos)
{
	char buf[256];
	int recvd = recv(_pollfdClients[pos].fd, buf, sizeof buf, 0);

	if (recvd < 0)
		std::cout << "Error recv()" << std::endl;
	else if (recvd == 0) {  // == disconnected
		delete _clients[pos - 1];
		_pollfdClients.erase(_pollfdClients.begin() + pos);
		for (size_t i = 0; i < _channels.size(); i++) {
			_channels[i].rmMember(_clients[pos - 1]);
			if (_channels[i].getMembers().size() == 0)
				_channels.erase(_channels.begin() + i);
		}
		_clients.erase(_clients.begin() + pos - 1);
		std::cout << "Client n" << pos << " disconnected" << std::endl;
	}
	else {
		buf[recvd] = '\0';
		std::cout << "client n" << pos << ": " << buf << std::endl;
		
		_clients[pos - 1]->addBufmsg(buf);
		if (_clients[pos - 1]->getBufmsg().find("\r\n") != std::string::npos) {
			_msgReceived = _clients[pos - 1]->getBufmsg();
			_clients[pos - 1]->rmBufmsg();
		}
	}
}

void	Server::completeRegistration(int pos)
{
	char buf[256];
	int recvd = recv(_pollfdClients[pos].fd, buf, sizeof buf, 0);

	if (recvd < 0)
		std::cout << "Error recv()" << std::endl;
	else if (recvd == 0) {  // == disconnected
		delete _clients[pos - 1];
		_pollfdClients.erase(_pollfdClients.begin() + pos);
		_clients.erase(_clients.begin() + pos - 1);
		std::cout << "Client n" << pos << " disconnected" << std::endl;
	}
	else {
		buf[recvd] = '\0';
		std::cout << "client n" << pos << ": " << buf << std::endl;
		
		_clients[pos - 1]->addBufmsg(buf);
		if ((_clients[pos - 1]->getUser().empty()
			&& _clients[pos - 1]->getBufmsg().find("\r\nNICK ") != std::string::npos
			&& _clients[pos - 1]->getBufmsg().find("\r\nUSER ") != std::string::npos)
			|| (!_clients[pos - 1]->getUser().empty()
			&& _clients[pos - 1]->getBufmsg().find("\r\n") != std::string::npos)) {
			_msgReceived = _clients[pos - 1]->getBufmsg();
			_clients[pos - 1]->rmBufmsg();
		}
	}
}

void	Server::deleteClient(int pos)
{
	delete _clients[pos];
	_clients.erase(_clients.begin() + pos);
	_pollfdClients.erase(_pollfdClients.begin() + pos + 1);
}

void	Server::handlePollerr(int pos)
{
	if (pos == 0)
		exitServer = true;
	else {
		for (size_t i = 0; i < _channels.size(); i++) {
			_channels[i].rmMember(_clients[pos - 1]);
			if (_channels[i].getMembers().size() == 0) {
				_channels.erase(_channels.begin() + i);
				i--;
			}
		}
		_clients.erase(_clients.begin() + pos - 1);
	}
}
