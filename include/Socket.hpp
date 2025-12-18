/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:09:03 by ikulik            #+#    #+#             */
/*   Updated: 2025/12/18 18:34:56 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP
# define MAX_CONNS 10

class Socket
{
	private:
		int					mainSocketFd;

		int	SetSocketAddr(int socket_fd, int port);
		int	AddSocketFlags(int socket_fd, int flags);
	public:
		int		GetMainSocketFd();
		void	CloseMainSocket();
		int		OpenMainSocket(int port);
};

#endif
