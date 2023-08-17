/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 14:41:33 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/09 17:10:39 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 3) {
		std::cout << "Expected: ./ircserv <port> <password>" << std::endl;
		return (1);
	}

	std::cout << "port: " << argv[1] << ", pswd: " << argv[2] << std::endl;

	Server server;

	try
	{
		server.getListenerSocket();
		server.mainLoop();
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}
