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

#include <sys/epoll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include "routers/Socket.hpp"
#include "utils/EpollWrappers.hpp"
#include "utils/Basics.hpp"

Socket::Socket(): EpollConent(ETYPE_SOCKET), mainSocketFd(-1), maxFds(DEF_MAX_CONNS){};

Socket::Socket(const ConfigMain& config): EpollConent(ETYPE_SOCKET), mainSocketFd(-1), maxFds(config.connectionsMax) {}

Socket::~Socket()
{
	CloseSocket();
};

void	Socket::AddServerConfig(const ServerConfig* cfg)
{
	if (cfg)
		serverConfigs.push_back(cfg);
}

const std::vector<const ServerConfig*>&	Socket::GetServerConfigs() const
{
	return serverConfigs;
}

int	Socket::OpenMainSocket(int port)
{
	return OpenMainSocket(IpPort(INADDR_ANY, port));
}

int	Socket::OpenMainSocket(IpPort address)
{
	int	errorCode = 0;

	mainSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (mainSocketFd < 0)
		return (E_SOCKET_CREATE);
	int opt = 1;
	setsockopt(mainSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	errorCode = SetSocketAddr(mainSocketFd, address);
	if (errorCode != E_SUCCESS)
		return (errorCode);
	errorCode = AddSocketFlags(mainSocketFd, O_NONBLOCK);
	if (errorCode != E_SUCCESS)
		return (errorCode);
	errorCode = listen(mainSocketFd, SOMAXCONN);
	if (errorCode != E_SUCCESS)
		return (E_SOCKET_CREATE);
	return (E_SUCCESS);
}

int	Socket::SetSocketAddr(int socket_fd, IpPort address)
{
	int	errorCode;
	struct sockaddr_in	newAddress;

	ft_bzero(&newAddress, sizeof(sockaddr_in));
	newAddress.sin_family = AF_INET;
	newAddress.sin_port = htons(address.second);
	newAddress.sin_addr.s_addr = htonl(address.first);
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

void	Socket::CloseSocket()
{
	if (mainSocketFd > -1)
	{
		std::set<int>::iterator	iter = openFds.begin();
		for (; iter != openFds.end(); iter++)
			close(*iter);
		openFds.clear();
		close(mainSocketFd);
		mainSocketFd = -1;
	}
}

int	Socket::GetMainSocketFd()
{
	return mainSocketFd;
}

//Checks for new connections returns fd
//On error returns -1 or -2
int	Socket::AcceptConnection()
{
	int	fd;

	if ((int)openFds.size() < maxFds)
	{
		fd = accept(mainSocketFd, NULL, NULL);
		if (fd < 0)
		{
			if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
				return -1;
			return -2;
		}
		fcntl(fd, F_SETFL, O_NONBLOCK);
		openFds.insert(fd);
		return fd;
	}
	return -1;
}

int	Socket::CloseConnection(int fd)
{
	int	result;

	if (openFds.size() == 0)
		return E_FAILURE;
	result = close(fd);
	if (result == 0)
		openFds.erase(fd);
	return result;
}
