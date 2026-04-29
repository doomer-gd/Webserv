/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Closer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtrofyme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 10:00:00 by vtrofyme          #+#    #+#             */
/*   Updated: 2026/04/29 10:00:00 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "services/Closer.hpp"
#include <sys/socket.h>
#include <unistd.h>

Closer::Closer(int fd): fd(fd), shutdownDone(false) {}

Closer::~Closer() {}

void	Closer::Initialize()
{
	if (!shutdownDone && fd >= 0)
	{
		shutdown(fd, SHUT_WR);
		shutdownDone = true;
	}
}

int	Closer::Execute()
{
	char	buf[4096];
	ssize_t	n;

	do
	{
		n = recv(fd, buf, sizeof(buf), 0);
	} while (n > 0);
	return FINISHED;
}

int	Closer::Exit()
{
	shutdownDone = false;
	return CS_DEAD;
}
