/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 09:42:37 by saguesse          #+#    #+#             */
/*   Updated: 2023/07/26 17:08:58 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <iostream>

int main(int argc, char** argv)
{
	if (argc != 3) {
		std::cout << "Expected: ./ircserv <port> <password>" << std::endl;
		return (1);
	}

	Server server(argv[1], argv[2]);
	try
	{
		server.getServerSocket();
		server.mainLoop();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	return (0);
}
