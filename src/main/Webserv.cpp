/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 15:17:46 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/13 17:58:19 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Config::Config():
	bufferSize(DEF_BUFFER_SIZE),
	numSockets(0),
	connectionsMax(DEF_MAX_CONNS)
{
}

int Webserv::exitCode_ = 0;
std::ostream&	Webserv::logStream = std::cerr;

Webserv::Webserv(){};
Webserv::~Webserv(){};

int	Webserv::ReadConfig() //needs implementation
{
	return 0;
}

const char*	g_errorMessage[NUM_ERRORS] =	{"",
							"webserv: error code 1",
							"webserv: wrong number of arguments",
							"webserv: socket creation error",
							"webserv: bind error",
							"webserv: socket flag error"};

int	Webserv::Exit(int errorCode)
{
	exitCode_ = errorCode;
	Webserv::Log(g_errorMessage[errorCode]);
	return (errorCode);
}

void	Webserv::Log(const std::string& message)
{
	DisplayTimestamp();
	logStream << message << std::endl;
}

void	Webserv::DisplayTimestamp(void)
{
	time_t now = time(NULL);
	struct tm *clock= localtime(&now);

	logStream << "[" << 1900 + clock->tm_year;
	logStream << std::setfill('0') << std::setw(2) << clock->tm_mon;
	logStream << std::setfill('0') << std::setw(2) << clock->tm_mday << "_";
	logStream << std::setfill('0') << std::setw(2) << clock->tm_hour;
	logStream << std::setfill('0') << std::setw(2) << clock->tm_min;
	logStream << std::setfill('0') << std::setw(2) << clock->tm_sec;
	logStream << "] ";
}
