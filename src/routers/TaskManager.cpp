/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:12:58 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/18 17:47:24 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "routers/TaskManager.hpp"
#include "routers/Socket.hpp"
#include "main/Webserv.hpp"
#include "utils/Codes.hpp"
#include "utils/Basics.hpp"
#include <ctime>
#include <signal.h>

extern volatile sig_atomic_t g_signal;

TaskManager::TaskManager(const ConfigMain& config_): poller(config_), config(config_), isOn(false)
{
	for (size_t idxServ = 0; idxServ < config.servers.size(); idxServ++)
	{
		int	sizePorts = config.servers[idxServ].portsArray.size();
		for (int idxSocket = 0; idxSocket < sizePorts; idxSocket++)
			socks.push_back(Socket(config, idxServ));
	}
	//clients.reserve(config.connectionsMax); doesn't work with regualr set
}

TaskManager::TaskManager(): config(), isOn(false)
{
	socks.reserve(config.numSockets);
	for (int i = 0; i < config.numSockets; i++)
		socks.push_back(Socket(config, 0));
	poller = Poller(config);
}

int	TaskManager::InnitializeServer(void)
{
	try
	{
		poller.CreatePoll();
		OpenSockets();
	}
	catch(const std::exception& e)
	{
		Webserv::Log("Webserver innitialization failed: " + std::string(e.what()));
		return E_FAILURE;
	}
	return E_SUCCESS;
}

int	TaskManager::OpenSockets()
{
	int		errorCode;
	int		failureCode = E_SUCCESS;
	bool	hasSuccess = false;

	for (size_t i = 0; i < socks.size(); i++)
	{
		errorCode = socks[i].OpenMainSocket(config.socketPorts[i]);
		if (errorCode != E_SUCCESS)
		{
			Webserv::Log("Error openning socket at port: " + toString(config.socketPorts[i]));
			failureCode = errorCode;
		}
		else
		{
			hasSuccess = true;
			Webserv::Log("Socket openned successfully at port: " + toString(config.socketPorts[i]));
		}
	}
	return HandleInitResult(hasSuccess, failureCode);
}

int	TaskManager::HandleInitResult(bool hasSuccess, int failureCode)
{
	int	result = E_SUCCESS;

	if (hasSuccess && failureCode != E_SUCCESS)
		result = E_SOCKET_CREATE;
	else if (!hasSuccess)
	{
		Webserv::exitCode_ = E_SOCKET_CREATE;
		throw Webserv::Except("error on all sockets");
	}
	return result;
}

int	TaskManager::StartMainLoop()
{
	isOn = true;
	try
	{
		while (isOn && g_signal == 0)
		{
			RunPolledEvents();
			ExecuteCommands();
			CheckTimeouts();
		}
	}
	catch(const std::exception& e)
	{
		Webserv::Log(e.what());
		return E_FAILURE;
	}
	Webserv::Log("Server shutting down");
	CleanupAllClients();
	return E_SUCCESS;
}

int	TaskManager::OpenNewConnections(Socket* sock)
{
	int	fdNewClient = -1;

	if (clients.size() >= (size_t)config.connectionsMax)
		return E_FAILURE;
	while (clients.size() < (size_t)config.connectionsMax)
	{
		fdNewClient = sock->AcceptConnection();
		if (fdNewClient < 0)
			return E_FAILURE;
		AddClient(fdNewClient, *sock);
	}
	return E_SUCCESS;
}

int	TaskManager::AddClient(int fd, Socket& sock)
{
	try
	{
		AConnection* conn = new Connection(&sock);
		conn->OpenConnection(fd);
		const ServerConfig* srvConf = NULL;
		srvConf = &(config.servers[sock.GetServerIndex()]);
		Client* client = new Client(conn, config, srvConf);
		clients.insert(client);
		poller.AddFd(client->GetFd(), EPOLLIN | EPOLLET, client);
	}
	catch(const std::exception& e)
	{
		Webserv::Log(e.what());
		return E_FAILURE;
	}
	return E_SUCCESS;
}

int	TaskManager::RunPolledEvents(void)
{
	int				numNewEvents;
	e_event_t		event;
	EpollConent*	content;
	Client*			client;

	numNewEvents = poller.Poll();
	for (int i = 0; i < numNewEvents; i++)
	{
		event = poller.GetEvent(i);
		content = static_cast<EpollConent*>(event.data.ptr);
		if (content->type == ETYPE_CLIENT)
		{
			client = static_cast<Client*>(event.data.ptr);
			if (clients.find(client) == clients.end())
				continue;
			if (event.events & (EPOLLHUP | EPOLLERR))
			{
				poller.RemoveFd(client->GetFd());
				clients.erase(client);
				delete client;
				continue;
			}
			HandleClientUpdate(client);
		}
		else if (content->type == ETYPE_SOCKET)
		{
			OpenNewConnections(static_cast<Socket*>(content));
		}
	}
	return 0;
}

int	TaskManager::ExecuteCommands(void)
{
	Client*		client;

	while (!queueExec.empty())
	{
		client = queueExec.front();
		if (clients.find(client) != clients.end())
			HandleClientUpdate(client);
		queueExec.pop();
	}
	return 0;
}

int	TaskManager::HandleClientUpdate(Client* client)
{
	ClientState	nextState;

	nextState = client->UpdateState();
	switch (nextState)
	{
	case CS_SENDING:
		poller.SetFdFlags(client->GetConnection()->GetFd(), EPOLLOUT | EPOLLET, client);
		break;
	case CS_EXEC_REQUEST:
		queueExec.push(client);
		break;
	case CS_READING_HEADER:
		poller.SetFdFlags(client->GetConnection()->GetFd(), EPOLLIN | EPOLLET, client);
		break;
	case CS_DEAD:
		poller.RemoveFd(client->GetFd());
		clients.erase(client);
		delete client;
		break;
	default:
		break;
	}
	return 0;
}

void	TaskManager::CheckTimeouts(void)
{
	time_t now = time(NULL);
	std::set<Client*>::iterator it = clients.begin();
	std::vector<Client*> toRemove;

	while (it != clients.end())
	{
		Client* client = *it;
		if (difftime(now, client->GetLastActivity()) > 60.0)
			toRemove.push_back(client);
		++it;
	}
	for (size_t i = 0; i < toRemove.size(); i++)
	{
		poller.RemoveFd(toRemove[i]->GetFd());
		clients.erase(toRemove[i]);
		delete toRemove[i];
	}
}

void	TaskManager::CleanupAllClients(void)
{
	std::set<Client*>::iterator it = clients.begin();
	while (it != clients.end())
	{
		Client* client = *it;
		poller.RemoveFd(client->GetFd());
		delete client;
		++it;
	}
	clients.clear();
}

void	TaskManager::RemoveClient(Client* client)
{
	poller.RemoveFd(client->GetFd());
	clients.erase(client);
	queueTimeout.erase(mapTimeout[client]);
	mapTimeout.erase(client);
	delete client; //for the moment deleting them, better to have a pre-allocated array
}

//Adds to queue or refreshes the value. Delay in seconds
int	TaskManager::AddToTimeoutQueue(Client* client, time_t delay)
{
	MapClientTimeout::iterator	iterMap;
	TimeoutQueue::iterator		iterSet;
	time_t	timeout = time(NULL) + delay;

	iterMap = mapTimeout.find(client);
	if (iterMap != mapTimeout.end())
		queueTimeout.erase(iterMap->second);
	iterSet = queueTimeout.insert(std::pair<time_t, Client*>(timeout, client)).first;
	mapTimeout[client] = iterSet;
	return 0;
}
