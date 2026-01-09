/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 14:06:44 by ikulik            #+#    #+#             */
/*   Updated: 2026/01/09 17:23:53 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Client::Client(): connection(NULL), e_currentState(CS_NUM_STATES) {};

Client::Client(Connection* connection, Config& config):  e_currentState(CS_READING_HEADER), connection(connection)
{
	buffer = std::string(config.bufferSize, 0);
	SetUpStates(config);
};

Client::Client(const Client& other)
{
	*this = other;
}

Client::~Client() {};

Client&	Client::operator=(const Client& other)
{
/* 	socket_fd = other.GetSocketFd();
	e_currentState = other.GetEnumState();
	currentState = other.GetCurrentState(); */
}

int	Client::GetSocketFd( void ) const
{
	return -1;
}

ClientState	Client::GetEnumState( void ) const
{
	return e_currentState;
}

IState*		Client::GetCurrentState ( void ) const
{
	return currentState;
}

