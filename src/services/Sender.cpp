/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sender.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 16:18:08 by ikulik            #+#    #+#             */
/*   Updated: 2026/01/27 16:34:05 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Sender::Sender(std::string& buffer): buffer(buffer){};

Sender::~Sender(){};


void	Sender::Initialize()
{
	std::cout << "Initializing sender" << std::endl;
}
int	Sender::Execute()
{
	std::cout << "Executing sender" << std::endl;
	return 0;
}

ClientState	Sender::Exit()
{
	std::cout << "Exiting sender" << std::endl;
	return CS_SENDING;
}
