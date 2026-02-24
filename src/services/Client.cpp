/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 14:06:44 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/18 17:30:56 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

IState::~IState(){};

Client::Client():	currentState(NULL),
					connection(NULL),
					command(NULL),
					e_currentState(CS_NUM_STATES),
					isReady(true) {};

Client::Client(AConnection* connection, const Config& config):
	connection(connection), e_currentState(CS_READING_HEADER), isReady(true)
{
	buffer.reserve(config.bufferSize);
	bufferSize = config.bufferSize;
	command = new Command();
};

Client::Client(AConnection* connection, const Config& config, std::vector<IState*>& states):
	connection(connection), e_currentState(CS_READING_HEADER), isReady(true)
{
	buffer.reserve(config.bufferSize);
	bufferSize = config.bufferSize;
	command = new Command();\
	this->states = states;
};

Client::~Client()
{
	CleanUpStates();
	safeDelete(command);
	if (connection != NULL)
	{
		connection->CloseConnection();
		delete connection;
	}
};

int	Client::GetFd( void ) const
{
	if (connection != NULL)
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
	states[CS_EXEC_REQUEST] = new Executer(buffer, *command);
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

	if (isReady == false)
		return CS_NUM_STATES;
	status = currentState->Execute();
	if (status == FINISHED)
	{
		nextState = currentState->Exit();
		currentState = states[nextState];
		currentState->Initialize();
	}
	return nextState;
}

