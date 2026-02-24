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
	Webserv::Log("Initializing sender");
}
int	Sender::Execute()
{
	Webserv::Log("Executing sender: buffer: " + buffer);
	return 0;
}

ClientState	Sender::Exit()
{
	Webserv::Log("Exiting sender");
	return CS_SENDING;
}
