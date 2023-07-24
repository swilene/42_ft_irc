/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 09:42:37 by saguesse          #+#    #+#             */
/*   Updated: 2023/07/21 15:38:29 by saguesse         ###   ########.fr       */
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
	// 

	return (0);
}
