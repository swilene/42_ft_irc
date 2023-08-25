#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <cstring>
#include <ctime>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#define WELCOME(networkname, nick) (":Welcome to the " + networkname + " Network, " + nick + "\r\n")
#define YOURHOST(servername, version) (":Your host is " + servername + ", running version " + version + "\r\n")
#define CREATED(time) (":This server was created " + time + "\r\n")
#define MYINFO(servername, version) ("Irssi " + servername + " " + version + " io itkol\r\n")
#define ISUPPORT "Irssi CHANNELLEN=32 NICKLEN=9 TOPICLEN=307 :are supported by this server\r\n"

void sendNumericReply(int clientSocket, int numericCode, const std::string& message) {
    std::string reply = ":" + std::string("your_server_name") + " "
		+ std::to_string(numericCode) + " " + message + "\r\n";
    send(clientSocket, reply.c_str(), reply.length(), 0);
}


void	rpl1_4(int clientSocket)
{
	sleep(1);
	char buf[3000];
	ssize_t recvd = recv(clientSocket, buf, sizeof(buf) - 1, 0);

	if (recvd <= 0) {
		std::cout << "recv failed" << std::endl;
		return ;
	}

	std::cout << buf << std::endl;

	// std::string networkname = "Internet Relay";
	// std::string servername = "127.0.0.1";
	// std::string version = "v1.2.3-1ubuntu4";
	// auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    // struct std::tm* timeInfo;
    // char buffer[80];
    // timeInfo = std::localtime(&currentTime);
    // std::strftime(buffer, sizeof(buffer), "%m-%d-%Y %X", timeInfo);
	// std::string time = buffer;
	std::string nick = buf;
	nick = nick.substr(nick.find("NICK ", 0) + 5, std::string::npos);
	nick = nick.substr(0, nick.find("\r\n", 0));
	if (clientSocket > 4)
		nick += clientSocket + 48;
	std::string user = buf;
	user = user.substr(user.find("USER ", 0) + 5, std::string::npos);
	user = user.substr(0, user.find(" ", 1));

	// RPL_WELCOME fonctionnel
	std::string reply = ":" + std::string("127.0.0.1") + " 001 " + nick + " Welcome to the Internet Relay Network "
		+ nick + "!" + user + "@127.0.0.1" + "\r\n";
	send(clientSocket, reply.c_str(), reply.size(), 0);
	//// useless
	// std::string msg = WELCOME(networkname, nick);
	// send(clientSocket, msg.c_str(), msg.size(), 0);
	// msg = YOURHOST(servername, version);
	// send(clientSocket, msg.c_str(), msg.size(), 0);
	// msg = CREATED(time);
	// send(clientSocket, msg.c_str(), msg.size(), 0);
	// msg = MYINFO(servername, version);
	// send(clientSocket, msg.c_str(), msg.size(), 0);
	// msg = ISUPPORT;
	// send(clientSocket, msg.c_str(), msg.size(), 0);
	// send(clientSocket, "motd\r\n", 6, 0);
}


int main(void)
{
	// init sockaddr
	struct sockaddr_in	servAddress;
	servAddress.sin_family = AF_INET;
	servAddress.sin_addr.s_addr = INADDR_ANY;
	servAddress.sin_port = htons(6667);		// pass port as argv
	socklen_t			servAddressLen = sizeof(servAddress);

	// init listening socket
	int  listenSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (listenSocket < 0) {
		std::cout << "listenSocket socket() failed" << std::endl;
		return 1;
	}
	if (bind(listenSocket, (struct sockaddr *)&servAddress, servAddressLen) < 0) {
		std::cout << "bind() failed" << std::endl;
		return 1;
	}
	if (listen(listenSocket, 64) < 0) {   // no backlog ?
		std::cout << "listen() failed" << std::endl;
		return 1;
	}

	std::vector<int>	clientSockets;
	std::vector<pollfd>	pollfds;

	// create listenSocket pollfd
	pollfd	listenPollfd;
	listenPollfd.fd = listenSocket;
	listenPollfd.events = POLLIN;
	pollfds.push_back(listenPollfd);

	while (1) {
		int	pollres = poll(pollfds.data(), pollfds.size(), -1);

		if (pollres < 0) {
			std::cout << "poll() failed" << std::endl;
			break;
		}

		// if data in listen socket
		if (pollfds[0].revents & POLLIN) {
			int clientSocket = accept(listenSocket, (struct sockaddr *)&servAddress, &servAddressLen);

			if (clientSocket > 0) {
				// add new client to pollfds
				pollfd	newclient;
				newclient.fd = clientSocket;
				newclient.events = POLLIN | POLLOUT;
				pollfds.push_back(newclient);
				clientSockets.push_back(clientSocket);

				// SEND RPL 1-4 ?
				rpl1_4(clientSocket);
			}
		}

		for (size_t i = 1; i < pollfds.size(); i++) {
			// if data in client socket
			if (pollfds[i].revents & POLLIN) {
				char buf[3000];
                ssize_t recvd = recv(pollfds[i].fd, buf, sizeof(buf) - 1, 0);

                if (recvd > 0) {
					// check for /r/n
                    buf[recvd] = '\0';
                    std::cout << "Client n" << i << ": " << buf << "[" << recvd << "]" << std::endl;
					// send to other clients
					for (size_t j = 0; j < clientSockets.size(); j++) {
						if (j != i - 1) {
							std::string	msg = "[";
							msg += (int)i + 48;
							msg += "]: ";
							msg += buf;
							send(clientSockets[j], msg.c_str(), recvd + 5, 0);
						}
					}
					// PONG (fonctionnel)
					std::string msg = buf;
					if (msg.find("PING", 0) != std::string::npos) {
						send(clientSockets[i - 1], "PONG 127.0.0.1\r\n", 16, 0);
						std::cout << "sent pong to n" << i << std::endl;
					}
                } else if (recvd == 0) {
                    std::cout << "Client n" << i << " disconnected" << std::endl;
                    close(pollfds[i].fd);
                    // Remove the clientSocket from the lists
                    pollfds.erase(pollfds.begin() + i);
                    clientSockets.erase(clientSockets.begin() + i - 1);
                    --i;
                } else {
                    std::cout << "recv() failed on Client n" << i << std::endl;
                    close(pollfds[i].fd);
                    // Remove the clientSocket from the lists
                    pollfds.erase(pollfds.begin() + i);
                    clientSockets.erase(clientSockets.begin() + i - 1);
                    --i;
				}
			}
		}
		// if all disconnected
		if (!clientSockets.size())
			break;
	}

	close(listenSocket);
	return 0;
}


	//// SOLO CLIENT
	// int clientSocket = accept(listenSocket, (struct sockaddr *)&servAddress, &servAddressLen);
	// // CONNECT A CLIENT
	// while (1) {
	// 	// sleep(1);
	// 	char buf[3000];
    //     ssize_t recvd = recv(clientSocket, buf, sizeof(buf) - 1, 0);

    //     if (recvd > 0) {
    //         buf[recvd] = '\0';
    //         std::cout << buf << "(" << recvd << ")" << std::endl;
    //     } else if (recvd == 0) {
    //         std::cout << "client disconnected" << std::endl;
    //         break;
    //     } else {
    //         std::cout << "recv() failed" << std::endl;
    //         break;
    //     }
	// }
