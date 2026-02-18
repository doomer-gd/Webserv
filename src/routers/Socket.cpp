/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 15:49:54 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/18 17:23:15 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Socket::Socket(): mainSocketFd(-1), numFds(0), maxFds(DEF_MAX_CONNS){};

Socket::Socket(const Config& config): mainSocketFd(-1), numFds(0)
{
	maxFds = config.connectionsMax; //should have a limit per socket instead of this
}

Socket::~Socket(){};

int	Socket::OpenMainSocket(int port)
{
	int	errorCode;

	mainSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (mainSocketFd < 0)
		return (E_SOCKET_CREATE);
	errorCode = SetSocketAddr(mainSocketFd, port);
	if (errorCode != E_SUCCESS)
		return (errorCode);
	errorCode = AddSocketFlags(mainSocketFd, O_NONBLOCK);
	if (errorCode != E_SUCCESS)
		return (errorCode);
	errorCode = listen(mainSocketFd, DEF_MAX_CONNS);
	if (errorCode != E_SUCCESS)
		return (E_SOCKET_CREATE);
	return (E_SUCCESS);
}

int	Socket::SetSocketAddr(int socket_fd, int port)
{
	int	errorCode;
	struct sockaddr_in	newAddress;

	ft_bzero(&newAddress, sizeof(sockaddr_in));
	newAddress.sin_family = AF_INET;
	newAddress.sin_port = htons(port);
	newAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	errorCode = bind(socket_fd, (sockaddr *)&newAddress, sizeof(sockaddr_in));
	if (errorCode != 0)
		return (E_BIND_ERROR);
	return (E_SUCCESS);
}

int	Socket::AddSocketFlags(int socket_fd, int flags)
{
	int	oldFlags;

	oldFlags = fcntl(socket_fd, F_GETFL);
	if (oldFlags < 0)
		return (E_SOCKET_FLAG);
	oldFlags |= flags;
	oldFlags = fcntl(socket_fd, F_SETFL, oldFlags);
	if (oldFlags < 0)
		return (E_SOCKET_FLAG);
	return (E_SUCCESS);
}

void	Socket::CloseMainSocket()
{
	close(mainSocketFd);
}

int	Socket::GetMainSocketFd()
{
	return mainSocketFd;
}

int	Socket::AcceptConnection()
{
	int	fd;

	if (numFds < maxFds)
	{
		fd = accept(mainSocketFd, NULL, NULL);
		if (fd < 0)
		{
			if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
				return -1;
			return -2;
		}
		numFds++;
		return fd;
	}
	return -1;
}

int	Socket::CloseConnection(int fd)
{
	int	result;

	if (numFds < 1)
		return E_FAILURE;
	result = close(fd);
	if (result == 0)
		numFds--;
	return result;
}
