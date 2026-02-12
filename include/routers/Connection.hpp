/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:02:30 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/12 16:28:21 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP
# include "main.hpp"

class AConnection
{
	protected:
		int	fd;
	public:
		virtual int	OpenConnection(int fd) = 0;
		virtual int	CloseConnection() = 0;
		int			GetFd(void) const;

};

class Connection: public AConnection
{
	protected:
		Socket*	sock;
	public:
		Connection();
		Connection(Socket* sock);

		int	OpenConnection(int fd);
		int	CloseConnection();
};

#endif
