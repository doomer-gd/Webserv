/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 17:12:23 by ikulik            #+#    #+#             */
/*   Updated: 2025/12/23 17:26:58 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

int	Connection::OpenConnection(int fd)
{
	this->fd = fd;
	return 0;
}

int	Connection::CloseConnection()
{
	return sock->CloseConnection(fd);
}
