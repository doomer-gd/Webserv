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
					serverConfig(NULL),
					e_currentState(CS_NUM_STATES),
					isReady(true) {}

Client::Client(AConnection* connection, const Config& config,
	const ServerConfig* serverConfig):
	currentState(NULL), connection(connection), serverConfig(serverConfig),
	e_currentState(CS_READING_HEADER), isReady(true)
{
	buffer.reserve(config.bufferSize);
	bufferSize = config.bufferSize;
	InnitializeStates(config);
	SetState(CS_READING_HEADER);
}

Client::~Client()
{
	CleanUpStates();
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
	Parser*	parser = new Parser(buffer, config, this);
	parser->LinkRequest(&request);
	states[CS_READING_HEADER] = parser;
	states[CS_READING_BODY] = new Reader(buffer);
	states[CS_EXEC_REQUEST] = new Executer(buffer, this, serverConfig);
	states[CS_SENDING] = new Sender(buffer);
}

void	Client::CleanUpStates(void)
{
	for (size_t i = 0; i < states.size(); i++)
		safeDelete(states[i]);
}

HttpRequest&	Client::GetRequest()
{
	return request;
}

std::string&	Client::GetBuffer()
{
	return buffer;
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
	else if (status == ERROR)
	{
		buffer = "HTTP/1.1 400 Bad Request\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: 50\r\n"
			"Connection: close\r\n\r\n"
			"<html><body><h1>400 Bad Request</h1></body></html>";
		nextState = CS_SENDING;
		currentState = states[CS_SENDING];
		currentState->Initialize();
	}
	return nextState;
}

