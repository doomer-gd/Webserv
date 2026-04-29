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

#include "main/main.hpp"
#include "services/CgiHandler.hpp"
#include <signal.h>
#include <sys/wait.h>

IState::~IState(){};

Client::Client():	EpollConent(ETYPE_CLIENT),
					currentState(NULL),
					connection(NULL),
					serverConfig(NULL),
					e_currentState(CS_NUM_STATES),
					lastActivity(time(NULL)),
					cgiPid(-1) {}

Client::Client(AConnection* connection, const ConfigMain& config,
	const std::vector<const ServerConfig*>& candidates): EpollConent(ETYPE_CLIENT),
	currentState(NULL), connection(connection), candidateConfigs(candidates),
	serverConfig(candidates.empty() ? NULL : candidates[0]),
	e_currentState(CS_READING_HEADER),
	lastActivity(time(NULL)), cgiPid(-1)
{
	buffer.reserve(config.bufferSize);
	bufferSize = config.bufferSize;
	InnitializeStates(config);
	SetState(CS_READING_HEADER);
}

Client::~Client()
{
	if (cgiPid > 0)
	{
		kill(cgiPid, SIGKILL);
		waitpid(cgiPid, NULL, 0);
		cgiPid = -1;
	}
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

const ServerConfig*	Client::GetServerConfig( void ) const
{
	return serverConfig;
}

static std::string	hostHeaderName(const HttpRequest& req)
{
	std::map<std::string, std::string>::const_iterator it
		= req.headers.find("host");
	if (it == req.headers.end())
		return "";
	std::string	host = it->second;
	size_t	colon = host.find(':');
	if (colon != std::string::npos)
		host = host.substr(0, colon);
	return host;
}

void	Client::SelectServerConfig(void)
{
	if (candidateConfigs.empty())
		return ;

	std::string	host = hostHeaderName(request);
	if (host.empty())
	{
		serverConfig = candidateConfigs[0];
		return ;
	}

	for (size_t i = 0; i < candidateConfigs.size(); i++)
	{
		const ServerConfig*	cfg = candidateConfigs[i];
		if (!cfg)
			continue;
		if (cfg->serverName == host)
		{
			serverConfig = cfg;
			return ;
		}
		for (size_t j = 0; j < cfg->serverNames.size(); j++)
		{
			if (cfg->serverNames[j] == host)
			{
				serverConfig = cfg;
				return ;
			}
		}
	}
	serverConfig = candidateConfigs[0];
}

void	Client::SetState(ClientState e_state)
{
	if (e_state >= states.size())
		return ;
	e_currentState = e_state;
	currentState = states[e_currentState];
}

void	Client::InnitializeStates(const ConfigMain& config)
{
	int clientFd = GetFd();
	states = std::vector<IState*>(CS_NUM_STATES);
	try
	{
		Parser*	parser = new Parser(buffer, config, clientFd);
		parser->LinkRequest(&request);
		states[CS_READING_HEADER] = parser;
		states[CS_EXEC_REQUEST] = new Executer(buffer, this);
		states[CS_CGI_WRITING] = new CgiInWriter();
		states[CS_CGI_READING] = new CgiState(buffer);
		states[CS_SENDING] = new Sender(buffer, clientFd);
		states[CS_CLOSING] = new Closer(clientFd);
	}
	catch (...)
	{
		CleanUpStates();
		throw;
	}
}

void	Client::CleanUpStates(void)
{
	for (size_t i = 0; i < states.size(); i++)
		safeDelete(&(states[i]));
}

HttpRequest&	Client::GetRequest()
{
	return request;
}

std::string&	Client::GetBuffer()
{
	return buffer;
}

time_t	Client::GetLastActivity(void) const
{
	return lastActivity;
}

ClientState	Client::UpdateState(void)
{
	int			status;
	ClientState	nextState = CS_NUM_STATES;

	lastActivity = time(NULL);
	status = currentState->Execute();
	if (status == FINISHED)
	{
		nextState = (ClientState)currentState->Exit();
		if (nextState != CS_DEAD && nextState < states.size() && states[nextState])
		{
			currentState = states[nextState];
			currentState->Initialize();
		}
	}
	else if (status == ERROR)
	{
		buffer = "HTTP/1.1 400 Bad Request\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: 50\r\n"
			"Connection: close\r\n\r\n"
			"<html><body><h1>400 Bad Request</h1></body></html>";
		currentState->Exit();
		nextState = CS_SENDING;
		currentState = states[CS_SENDING];
		currentState->Initialize();
	}
	return nextState;
}

int	Client::GetCgiReadFd(void) const
{
	if (states.size() <= CS_CGI_READING)
		return -1;
	CgiState*	cgi = dynamic_cast<CgiState*>(states[CS_CGI_READING]);
	if (cgi)
		return cgi->GetPipeFd();
	return -1;
}

int	Client::GetCgiWriteFd(void) const
{
	if (states.size() <= CS_CGI_WRITING)
		return -1;
	CgiInWriter*	w = dynamic_cast<CgiInWriter*>(states[CS_CGI_WRITING]);
	if (w)
		return w->GetWriteFd();
	return -1;
}

void	Client::SetupCgi(int writeFd, int readFd, pid_t pid,
	const std::string* body)
{
	cgiPid = pid;
	if (states.size() > CS_CGI_WRITING)
	{
		CgiInWriter*	w = dynamic_cast<CgiInWriter*>(states[CS_CGI_WRITING]);
		if (w)
			w->Setup(writeFd, body);
	}
	if (states.size() > CS_CGI_READING)
	{
		CgiState*	cgi = dynamic_cast<CgiState*>(states[CS_CGI_READING]);
		if (cgi)
			cgi->Setup(readFd);
	}
}

void	Client::CloseCgiReadFd(void)
{
	if (states.size() <= CS_CGI_READING)
		return ;
	CgiState*	cgi = dynamic_cast<CgiState*>(states[CS_CGI_READING]);
	if (cgi)
		cgi->ClosePipe();
}

void	Client::CloseCgiWriteFd(void)
{
	if (states.size() <= CS_CGI_WRITING)
		return ;
	CgiInWriter*	w = dynamic_cast<CgiInWriter*>(states[CS_CGI_WRITING]);
	if (w)
		w->ClosePipe();
}

int	Client::DriveCgiRead(void)
{
	if (states.size() <= CS_CGI_READING || !states[CS_CGI_READING])
		return EXECUTING;
	lastActivity = time(NULL);
	return states[CS_CGI_READING]->Execute();
}

int	Client::DriveCgiWrite(void)
{
	if (states.size() <= CS_CGI_WRITING || !states[CS_CGI_WRITING])
		return EXECUTING;
	lastActivity = time(NULL);
	return states[CS_CGI_WRITING]->Execute();
}

void	Client::TransitionToSending(void)
{
	e_currentState = CS_SENDING;
	if (states.size() > CS_SENDING && states[CS_SENDING])
	{
		currentState = states[CS_SENDING];
		currentState->Initialize();
	}
}
