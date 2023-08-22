/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 14:41:33 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/22 14:44:02 by saguesse         ###   ########.fr       */
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

	try
	{
		Server server(argv[1], argv[2]);
		server.getListenerSocket();
		server.mainLoop();
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}
