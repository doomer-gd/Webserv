/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:16:56 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/04 18:32:10 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Poller::Poller(): fdRegistry(-1), events(0){};

Poller::Poller(Config& config): fdRegistry(-1)
{
	maxConnections = config.maxConnections;
	events.reserve(maxConnections);
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
