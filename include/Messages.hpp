/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Messages.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saguesse <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 12:08:18 by saguesse          #+#    #+#             */
/*   Updated: 2023/08/08 12:16:20 by saguesse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <iostream>
#include <string>

//cpp01 ex05

class Messages
{
	private:
		void connectionMsg() const;

	public:
		Messages();
		~Messages();

		void sendMsg(std::string msg) const;
};

#endif
