/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:16:56 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/12 17:10:40 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Poller::Poller(): fdRegistry(-1), events(0), connectionsCurrent(0){};

Poller::Poller(Config& config): fdRegistry(-1), connectionsCurrent(0)
{
	connectionsMax = config.connectionsMax;
	events = std::vector<struct epoll_event>(connectionsMax);
}

Poller::~Poller()
{
	if (fdRegistry > -1)
		close(fdRegistry);
}

struct epoll_event&	Poller::GetEvent(int index)
{
	return events[index];
}

int	Poller::CreatePoll(void)
{
	return epoll_create1(0);
}

int	Poller::AddFd(int fd, int mask, void* data)
{
	e_event_t	event;
	int			exitCode;

	if (connectionsCurrent == connectionsMax)
		return E_FAILURE;
	event.events = mask;
	event.data.ptr = data;
	exitCode = epoll_ctl(fdRegistry, EPOLL_CTL_ADD, fd, &event);
	if (exitCode == 0)
		connectionsCurrent++;
	return exitCode;
}

int	Poller::RemoveFd(int fd)
{
	return epoll_ctl(fdRegistry, EPOLL_CTL_DEL, fd, NULL);
}

int	Poller::AddConnection(AConnection* connection, int mask)
{
	if (connection == NULL)
		return E_FAILURE;
	return AddFd(connection->GetFd(), mask, connection);
}

int	Poller::RemoveConnection(AConnection* connection)
{
	if (connection == nullptr)
		return E_FAILURE;
	return epoll_ctl(fdRegistry, EPOLL_CTL_DEL, connection->GetFd(), NULL);
}

int	Poller::AddClient(Client* client, int mask)
{
	if (client == nullptr)
		return E_FAILURE;
	return AddConnection(client->GetConnection(), mask);
}

int	Poller::RemoveClient(Client* client)
{
	if (client == nullptr)
		return E_FAILURE;
	return RemoveConnection(client->GetConnection());
}

int	Poller::SetFdFlags(int fd, int mask, void* data)
{
	e_event_t	event;

	event.events = mask;
	event.data.ptr = data;
	return epoll_ctl(fdRegistry, EPOLL_CTL_MOD, fd, &event);
}

int	Poller::Poll (void)
{
	numNewEvents = epoll_wait(fdRegistry, &events[0], connectionsCurrent, 0);
	return numNewEvents;
}

e_event_t	Poller::GetEventStatus(int index)
{
	return events[index];
}
