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

#include "main/main.hpp"
#include <ctime>

extern volatile sig_atomic_t g_signal;

TaskManager::TaskManager(const ConfigMain& config_): poller(config_), config(config_), isOn(false)
{
	for (size_t idxServ = 0; idxServ < config.servers.size(); idxServ++)
	{
		size_t	sizePorts = config.servers[idxServ].portsArray.size();
		for (size_t idxSocket = 0; idxSocket < sizePorts; idxSocket++)
		{
			socks.push_back(Socket(config, idxServ));
			flatPorts.push_back(config.servers[idxServ].portsArray[idxSocket]);
		}
	}
}

TaskManager::TaskManager(): config(), isOn(false)
{
	socks.reserve(config.numSockets);
	for (int i = 0; i < config.numSockets; i++)
	{
		socks.push_back(Socket(config));
	}
	poller = Poller(config);
}

int	TaskManager::InnitializeServer(void)
{
	int		errorCode;
	int		failureCode = E_SUCCESS;
	bool	hasSuccess = false;

	poller.CreatePoll();
	for (size_t i = 0; i < socks.size(); i++)
	{
		errorCode = socks[i].OpenMainSocket(flatPorts[i]);
		if (errorCode != E_SUCCESS)
		{
			Webserv::Log("Error openning socket at port: " + toString(flatPorts[i].second));
			failureCode = errorCode;
		}
		else
		{
			poller.AddFd(socks[i].GetMainSocketFd(), EPOLLIN, &socks[i]);
			listenPtrs.insert(&socks[i]);
			hasSuccess = true;
			Webserv::Log("Socket openned successfully at port: " + toString(flatPorts[i].second));
		}
	}
	return HandleInitResult(hasSuccess, failureCode);
}

int	TaskManager::HandleInitResult(bool hasSuccess, int failureCode)
{
	int	result = E_SUCCESS;

	if (hasSuccess && failureCode == E_SUCCESS)
		Webserv::Log("Server started successfully");
	else if (hasSuccess && failureCode != E_SUCCESS)
	{
		Webserv::Log("Server started with some sockets failing");
		result = E_SOCKET_CREATE;
	}
	else
	{
		Webserv::Log("Server failed to start due to socket error");
		result = E_FAILURE;
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

void	TaskManager::OpenNewConnections(Socket* sock)
{
	int	fdNewClient;

	if (clients.size() >= (size_t)config.connectionsMax)
		return;
	while (clients.size() < (size_t)config.connectionsMax)
	{
		fdNewClient = sock->AcceptConnection();
		if (fdNewClient < 0)
			break;
		AddClient(fdNewClient, *sock);
	}
}

int	TaskManager::AddClient(int fd, Socket& sock)
{
	try
	{
		AConnection* conn = new Connection(&sock);
		conn->OpenConnection(fd);
		const ServerConfig* srvConf = NULL;
		if (!config.servers.empty())
			srvConf = &(config.servers[sock.GetServerIndex()]);
		Client* client = new Client(conn, config, srvConf);
		clients.insert(client);
		poller.AddFd(client->GetFd(), EPOLLIN, client);
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
	int			numNewEvents;
	e_event_t	event;

	numNewEvents = poller.Poll();
	for (int i = 0; i < numNewEvents; i++)
	{
		event = poller.GetEvent(i);
		if (listenPtrs.count(event.data.ptr))
		{
			Socket*	sock = static_cast<Socket*>(event.data.ptr);
			OpenNewConnections(sock);
			continue;
		}
		Client*	client = static_cast<Client*>(event.data.ptr);
		if (clients.find(client) == clients.end())
			continue;
		if (client->GetCgiPipeFd() >= 0)
		{
			HandleClientUpdate(client);
			continue;
		}
		if (event.events & (EPOLLHUP | EPOLLERR))
		{
			poller.RemoveFd(client->GetFd());
			clients.erase(client);
			delete client;
			continue;
		}
		HandleClientUpdate(client);
	}
	return 0;
}

int	TaskManager::ExecuteCommands(void)
{
	Client*		client;

	while (!queueExec.empty())
	{
		client = queueExec.front();
		queueExec.pop();
		if (clients.find(client) != clients.end())
			HandleClientUpdate(client);
	}
	return 0;
}

int	TaskManager::HandleClientUpdate(Client* client)
{
	ClientState	nextState;

	nextState = client->UpdateState();
	switch (nextState)
	{
	case CS_CGI_READING:
	{
		poller.RemoveFd(client->GetFd());
		poller.AddFd(client->GetCgiPipeFd(), EPOLLIN, client);
		break;
	}
	case CS_SENDING:
	{
		int cgiFd = client->GetCgiPipeFd();
		if (cgiFd >= 0)
		{
			poller.RemoveFd(cgiFd);
			client->CloseCgiPipe();
			poller.AddFd(client->GetFd(), EPOLLOUT, client);
		}
		else
			poller.SetFdFlags(client->GetFd(), EPOLLOUT, client);
		break;
	}
	case CS_EXEC_REQUEST:
		queueExec.push(client);
		break;
	case CS_READING_HEADER:
		poller.SetFdFlags(client->GetFd(), EPOLLIN, client);
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
		int cgiFd = toRemove[i]->GetCgiPipeFd();
		if (cgiFd >= 0)
			poller.RemoveFd(cgiFd);
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
		int cgiFd = client->GetCgiPipeFd();
		if (cgiFd >= 0)
			poller.RemoveFd(cgiFd);
		poller.RemoveFd(client->GetFd());
		delete client;
		++it;
	}
	clients.clear();
}
