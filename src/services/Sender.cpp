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

#include "services/Sender.hpp"
#include "main/Webserv.hpp"
#include "utils/Codes.hpp"
#include <unistd.h>

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
	else if (n == 0)
		return FINISHED;
	if (bytesSent >= buffer.size())
		return FINISHED;
	return EXECUTING;
}

int	Sender::Exit()
{
	buffer.clear();
	return CS_CLOSING;
}
