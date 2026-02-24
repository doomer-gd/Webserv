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

#include "main.hpp"

TaskManager::TaskManager(const Config& config_): poller(config_), config(config_), isOn(false)
{
	socks = std::vector<Socket>(config.numSockets, Socket(config));
	//clients.reserve(config.connectionsMax); doesn't work with regualr set
}

TaskManager::TaskManager(): config(), isOn(false)
{
	socks.reserve(config.numSockets);
	for (size_t i = 0; i < config.numSockets; i++)
	{
		socks.push_back(Socket(config));
	}
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
			poller.AddFd(socks[i].GetMainSocketFd(), EPOLLIN | EPOLLET, &socks[i]);
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
		while (isOn)
		{
			OpenNewConnections();
			RunPolledEvents();
			ExecuteCommands();
			isOn = false; //test run
		}
	}
	catch(const std::exception& e)
	{
		Webserv::Log(e.what());
		return E_FAILURE;
	}
	return E_SUCCESS;
}

int	TaskManager::OpenNewConnections()
{
	int	fdNewClient;
	int	errorCode = E_SUCCESS;

	if (clients.size() >= config.connectionsMax)
		return E_FAILURE;
	for (size_t i = 0; i < socks.size(); i++)
	{
		fdNewClient = socks[i].AcceptConnection();
		if (fdNewClient > -1)
		{
			if (AddClient(fdNewClient, socks[i]) != E_SUCCESS)
			{
				errorCode = E_FAILURE;
				Webserv::Log("New client creation failure at socket:" + toString(socks[i].GetMainSocketFd()));
			}
		}
	}
	return errorCode;
}

int	TaskManager::AddClient(int fd, Socket& sock)
{
	try
	{
		AConnection* conn = new Connection(&sock);
		conn->OpenConnection(fd);
		const ServerConfig* srvConf = NULL;
		if (!config.servers.empty())
			srvConf = &config.servers[0];
		Client* client = new Client(conn, config, srvConf);
		clients.insert(client);
		poller.AddFd(client->GetFd(), EPOLLIN | EPOLLET, client); //catch errors here too
	}
	catch(const std::exception& e)
	{
		Webserv::Log("Failed to add client due to: " + std::string(e.what()));
		return E_FAILURE; //probably a more specific error needed
	}
	return E_SUCCESS;
}

int	TaskManager::RunPolledEvents(void)
{
	int			numNewEvents;
	e_event_t	event;
	Client*		client;

	numNewEvents = poller.Poll();
	for (int i = 0; i < numNewEvents; i++)
	{
		event = poller.GetEvent(i);
		client = static_cast<Client*>(event.data.ptr);
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


