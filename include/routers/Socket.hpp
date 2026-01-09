/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:09:03 by ikulik            #+#    #+#             */
/*   Updated: 2025/12/23 16:14:48 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

class Socket
{
	private:
		int	mainSocketFd;
		int	numFds;
		int	maxFds;

		int	SetSocketAddr(int socket_fd, int port);
		int	AddSocketFlags(int socket_fd, int flags);
	public:
		Socket();
		~Socket();

		int		GetMainSocketFd();
		int		AcceptConnection();
		int		CloseConnection(int fd);
		int		OpenMainSocket(int port);
		void	CloseMainSocket();
};

#endif
