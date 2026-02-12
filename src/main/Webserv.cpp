/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 15:17:46 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/12 15:03:39 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Config::Config():
	bufferSize(DEF_BUFFER_SIZE),
	numSockets(DEF_NUM_SOCKETS),
	connectionsMax(DEF_MAX_CONNS){};

int Webserv::exitCode_ = 0;

const char*	g_errorMessage[NUM_ERRORS] =	{"",
							"webserv: error code 1",
							"webserv: wrong number of arguments",
							"webserv: socket creation error",
							"webserv: bind error",
							"webserv: socket flag error"};

int	Webserv::Exit(ExitCode errorCode)
{
	exitCode_ = errorCode;
	std::cerr << g_errorMessage[errorCode] << std::endl;
	return (errorCode);
}
