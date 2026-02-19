/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:02:30 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/18 17:07:10 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP
# include "main.hpp"

class Socket;

class AConnection
{
	protected:
		int	fd;
	public:
		virtual int	OpenConnection(int fd) = 0;
		virtual int	CloseConnection() = 0;
		int			GetFd(void) const;
		virtual		~AConnection() = 0;

};

class Connection: public AConnection
{
	protected:
		Socket*	sock;
	public:
		Connection();
		~Connection();
		Connection(Socket* sock);

		int	OpenConnection(int fd);
		int	CloseConnection();
};

#endif
