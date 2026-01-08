/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 14:06:44 by ikulik            #+#    #+#             */
/*   Updated: 2026/01/08 14:50:48 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Client::Client(): socket_fd(-1), e_currentState(CS_NUM_STATES) {};

Client::Client(int socket_fd): socket_fd(socket_fd), e_currentState(CS_RECIEVING) {};

Client::Client(const Client& other)
{
	*this = other;
}

Client::~Client() {};

Client&	Client::operator=(const Client& other)
{
	socket_fd = other.GetSocketFd();
	e_currentState = other.GetEnumState();
	currentState = other.GetCurrentState();
}

int			Client::GetSocketFd( void ) const
{
	return socket_fd;
}

ClientState	Client::GetEnumState( void ) const
{
	return e_currentState;
}

IState		Client::GetCurrentState ( void ) const
{
	return currentState;
}

