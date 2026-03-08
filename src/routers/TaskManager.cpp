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

#include "../../include/routers/TaskManager.hpp"
#include "../../include/routers/Socket.hpp"
#include "../../include/main/Webserv.hpp"
#include "../../include/utils/Codes.hpp"
#include "../../include/utils/Basics.hpp"

TaskManager::TaskManager(const ConfigMain& config_): poller(config_), config(config_), isOn(false)
{
	socks = std::vector<Socket>(config.numSockets, Socket(config));
	for (int i = 0; i < config.numSockets; i++)
		socks[i].SetServerIndex(i);
	//clients.reserve(config.connectionsMax); doesn't work with regualr set
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
		int	cycles = 0;
		while (isOn && cycles < 2000000)
		{
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

int	TaskManager::OpenNewConnections(Socket* sock)
{
	int	fdNewClient = -1;
	int	errorCode = E_SUCCESS;

	if (clients.size() >= (size_t)config.connectionsMax)
		return E_FAILURE;
	while (clients.size() < (size_t)config.connectionsMax)
	{
		fdNewClient = sock->AcceptConnection();
		if (fdNewClient < 0)
			break;
		AddClient(fdNewClient, *sock);
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
			srvConf = &config.servers[sock.GetServerIndex()];
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
	int				numNewEvents;
	e_event_t		event;
	EpollConent*	content;

	numNewEvents = poller.Poll();
	for (int i = 0; i < numNewEvents; i++)
	{
		event = poller.GetEvent(i);
		content = static_cast<EpollConent*>(event.data.ptr);
		if (content->type == ETYPE_CLIENT)
			HandleClientUpdate(dynamic_cast<Client*>(content));
		else if (content->type == ETYPE_SOCKET)
			OpenNewConnections(dynamic_cast<Socket*>(content));
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


