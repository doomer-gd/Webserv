/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 14:06:44 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/04 17:55:05 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Client::Client(): connection(NULL), e_currentState(CS_NUM_STATES), isReady(false) {};

Client::Client(Connection* connection, Config& config):
	e_currentState(CS_READING_HEADER), connection(connection), isReady(false)
{
	buffer.reserve(config.bufferSize);
	bufferSize = config.bufferSize;
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


void	Client::SetUpStates(Config& config)
{
	states[CS_READING_HEADER] = new Parser(buffer, config, this);
	states[CS_READING_BODY] = new Reader(buffer);
	states[CS_EXEC_REQUEST] = new Executer(buffer, command);
	states[CS_SENDING] = new Sender(buffer);
}

int	Client::UpdateState(void)
{
	int	status;
	ClientState	nextState = CS_NUM_STATES;

	status = currentState->Execute();
	if (status == FINISHED)
	{
		nextState = currentState->Exit();
		currentState = states[nextState];
		currentState->Initialize();
	}
}

