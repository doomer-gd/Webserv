/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 17:12:23 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/12 16:29:17 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

int AConnection::GetFd(void) const
{
	return fd;
}

AConnection::~AConnection(){};

Connection::Connection(): sock(NULL){};


Connection::Connection(Socket* sock): sock(sock)
{
	fd = -1;
};

Connection::~Connection(){};

int	Connection::OpenConnection(int fd)
{
	this->fd = fd;
	return 0;
}

int	Connection::CloseConnection()
{
	if (sock != NULL)
		return sock->CloseConnection(fd);
	return E_FAILURE;
}
