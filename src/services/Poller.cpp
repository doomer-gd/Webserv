/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:16:56 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/17 14:58:45 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "services/Poller.hpp"
#include "main/Webserv.hpp"
#include <unistd.h>

Poller::Poller(): fdRegistry(-1), connectionsCurrent(0){};

Poller::Poller(const ConfigMain& config): fdRegistry(-1), connectionsCurrent(0)
{
	connectionsMax = config.connectionsMax;
	events = std::vector<struct epoll_event>(connectionsMax + config.numSockets);
}

Poller::~Poller()
{
	if (fdRegistry > -1)
		close(fdRegistry);
}

e_event_t&	Poller::GetEvent(int index)
{
	return events[index];
}

int	Poller::CreatePoll(void)
{
	fdRegistry = epoll_create(1);
	if (fdRegistry < 0)
	{
		throw Webserv::Except("epoll creation failure");
	}
	return 0;
}

int	Poller::AddFd(int fd, int mask, void* data)
{
	e_event_t	event;
	int			exitCode;

	if (connectionsCurrent == connectionsMax)
		return E_FAILURE; //needs better errorcode
	event.events = mask;
	event.data.ptr = data;
	exitCode = epoll_ctl(fdRegistry, EPOLL_CTL_ADD, fd, &event);
	if (exitCode == 0)
		connectionsCurrent++;
	return exitCode;
}

int	Poller::RemoveFd(int fd)
{
	int ret = epoll_ctl(fdRegistry, EPOLL_CTL_DEL, fd, NULL);
	if (ret == 0 && connectionsCurrent > 0)
		connectionsCurrent--;
	return ret;
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
	numNewEvents = epoll_wait(fdRegistry, &events[0], events.size(), 0);
	return numNewEvents;
}
