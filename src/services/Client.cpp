/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 14:06:44 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/17 14:52:09 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Client::Client(): connection(NULL), e_currentState(CS_NUM_STATES), isReady(false) {};

Client::Client(AConnection* connection, const Config& config):
	e_currentState(CS_READING_HEADER), connection(connection), isReady(false)
{
	buffer.reserve(config.bufferSize);
	bufferSize = config.bufferSize;
};

Client::Client(AConnection* connection, const Config& config, std::vector<IState*>& states): Client(connection, config)
{
	this->states = states;
}

Client::~Client()
{
	CleanUpStates();
	if (connection != nullptr)
	{
		connection->CloseConnection();
		delete connection;
	}
};

int	Client::GetFd( void ) const
{
	if (connection != nullptr)
		return connection->GetFd();
	return -1;
}

ClientState	Client::GetEnumState( void ) const
{
	return e_currentState;
}

IState*	Client::GetCurrentState ( void ) const
{
	return currentState;
}

AConnection*	Client::GetConnection( void ) const
{
	return connection;
}

void	Client::SetState(ClientState e_state)
{
	if (e_state >= states.size())
		return ;
	e_currentState = e_state;
	currentState = states[e_currentState];
}

void	Client::InnitializeStates(const Config& config)
{
	states = std::vector<IState*>(CS_NUM_STATES);
	states[CS_READING_HEADER] = new Parser(buffer, config, this);
	states[CS_READING_BODY] = new Reader(buffer);
	states[CS_EXEC_REQUEST] = new Executer(buffer, command);
	states[CS_SENDING] = new Sender(buffer);
}

void	Client::CleanUpStates(void)
{
	for (size_t i = 0; i < states.size(); i++)
		safeDelete(states[i]);
}

ClientState	Client::UpdateState(void)
{
	int			status;
	ClientState	nextState = CS_NUM_STATES;

	status = currentState->Execute();
	if (status == FINISHED)
	{
		nextState = currentState->Exit();
		currentState = states[nextState];
		currentState->Initialize();
	}
	return nextState;
}

