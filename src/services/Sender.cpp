/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sender.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 16:18:08 by ikulik            #+#    #+#             */
/*   Updated: 2026/01/27 16:34:05 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Sender::Sender(std::string& buffer, int fd): buffer(buffer), fd(fd), bytesSent(0){};

Sender::~Sender(){};

void	Sender::Initialize()
{
	bytesSent = 0;
}

int	Sender::Execute()
{
	if (bytesSent >= buffer.size())
		return FINISHED;
	ssize_t n = write(fd, buffer.c_str() + bytesSent, buffer.size() - bytesSent);
	if (n > 0)
		bytesSent += n;
	if (bytesSent >= buffer.size())
		return FINISHED;
	return EXECUTING;
}

ClientState	Sender::Exit()
{
	buffer.clear();
	return CS_DEAD;
}
