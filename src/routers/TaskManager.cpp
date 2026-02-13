/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 17:12:58 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/13 17:56:46 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

TaskManager::TaskManager(const Config& config_): config(config_), poller(config_), isOn(false)
{
	socks = std::vector<Socket>(config.numSockets, Socket(config));
	clients.reserve(config.connectionsMax);
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
	int		isFailure = E_SUCCESS;
	bool	partialSucces = false;

	for (int i = 0; i < socks.size(); i++)
	{
		errorCode = socks[i].OpenMainSocket(config.socketPorts[i]);
		if (errorCode != E_SUCCESS)
		{
			Webserv::Log("Error openning socket at port: " + config.socketPorts[i]);
			isFailure = errorCode;
		}
		else
			partialSucces = true;
	}
	if (partialSucces && isFailure == E_SUCCESS)
		Webserv::Log("Server started successfully");
	else if (partialSucces && isFailure != E_SUCCESS)
		Webserv::Log("Server started with some sockets failing");
	else
	{
		Webserv::Log("Server failed to start due to socket error");
		Webserv::Exit(E_SOCKET_CREATE);
	}
	return isFailure;
}

int	TaskManager::StartMainLoop()
{
	int	errorCode;

	isOn = true;
	while (isOn)
	{
		OpenNewConnections();
		RunPolledEvents();
		ExecuteCommands();
	}
}

int	TaskManager::OpenNewConnections()
{
	int	fdNewClient;
	int	errorCode = E_SUCCESS;

	if (clients.size() >= config.connectionsMax)
		return E_FAILURE;
	for (Socket& sock: socks)
	{
		fdNewClient = sock.AcceptConnection();
		if (fdNewClient > -1)
		{
			if (AddClient(fdNewClient, sock) != E_SUCCESS)
			{
				errorCode = E_FAILURE;
				Webserv::Log("New client creation failure at socket:" + sock.GetMainSocketFd());
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
		Client* client = new Client(conn, config);
		clients.insert(client);
		poller.AddClient(client, EPOLLIN | EPOLLET); //catch errors here too
	}
	catch(const std::exception& e)
	{
		Webserv::Log(e.what());
		return E_FAILURE; //probably a more specific error needed
	}
	return 0;
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
		poller.RemoveClient(client);
		clients.erase(client);
		delete client;
		break;
	default:
		break;
	}
}


