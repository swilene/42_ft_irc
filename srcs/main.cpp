/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 14:41:33 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/29 15:29:35 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <iostream>

bool exitServer = false;

void handleSignals(int signal)
{
	if (signal == SIGINT)
		exitServer = true;
}

int main(int argc, char **argv)
{
	if (argc != 3) {
		std::cout << "Expected: ./ircserv <port> <password>" << std::endl;
		return (1);
	}

	signal(SIGINT, handleSignals);
	
	Server server(argv[1], argv[2]);

	try
	{
		server.getListenerSocket();
		server.mainLoop();
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}
