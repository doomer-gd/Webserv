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

#include <csignal>
#include <netinet/in.h>
#include "main/Config.hpp"
#include "main/Webserv.hpp"
#include "utils/Basics.hpp"

volatile sig_atomic_t g_signal = 0;

ConfigMain::ConfigMain():
	bufferSize(DEF_BUFFER_SIZE),
	bodyBufferSize(DEF_BUFFER_SIZE),
	numSockets(DEF_NUM_SOCKETS),
	connectionsMax(DEF_MAX_CONNS),
	fdsMax(DEF_MAX_CONNS),
	socketPorts(std::vector<IpPort>(DEF_NUM_SOCKETS, IpPort(INADDR_ANY, DEF_PORT)))
{
	ServerConfig	defaultServer;
	LocationConfig	defaultLoc;

	defaultServer.port = DEF_PORT;
	defaultLoc.path = "/";
	defaultLoc.root = "test_site";
	defaultLoc.index = "index.html";
	defaultLoc.autoindex = true;
	defaultLoc.methods.insert("GET");
	defaultLoc.methods.insert("POST");
	defaultLoc.methods.insert("DELETE");
	defaultServer.locations.push_back(defaultLoc);

	LocationConfig	cgiLoc;

	cgiLoc.path = "/cgi-bin";
	cgiLoc.root = "test_site/cgi-bin";
	cgiLoc.cgiExtension = ".py";
	cgiLoc.cgiPath = "/usr/bin/python3";
	cgiLoc.methods.insert("GET");
	cgiLoc.methods.insert("POST");
	defaultServer.locations.push_back(cgiLoc);

	LocationConfig	uploadLoc;

	uploadLoc.path = "/upload";
	uploadLoc.root = "test_site/upload";
	uploadLoc.uploadStore = "test_site/upload";
	uploadLoc.methods.insert("GET");
	uploadLoc.methods.insert("POST");
	uploadLoc.methods.insert("DELETE");
	uploadLoc.autoindex = true;
	defaultServer.locations.push_back(uploadLoc);

	servers.push_back(defaultServer);
}

int Webserv::exitCode_ = 0;
std::ostream&	Webserv::logStream = std::cerr;
std::fstream	Webserv::logFile;

Webserv::Webserv(){};

Webserv::~Webserv(){};

const char*	g_errorMessage[NUM_ERRORS] =	{"Exited successfully",
							"Error code 1",
							"Wrong number of arguments",
							"Socket creation error",
							"Bind error",
							"Socket flag error",
							"Epoll error",
							"Config file error"};

int Webserv::OpenLogFile(const char* logFileName)
{
	logFile.open(logFileName, std::ios_base::out);
	if (logFile.fail())
		return E_FAILURE;
	return E_SUCCESS;
}


int	Webserv::Exit(int errorCode)
{
	exitCode_ = errorCode;
	if (errorCode >= 0 && errorCode < NUM_ERRORS)
		Webserv::Log(g_errorMessage[errorCode]);
	logFile.close();
	return (errorCode);
}

void	Webserv::Log(const std::string& message)
{
	DisplayTimestamp(logStream);
	logStream << "Webserv: " << message << std::endl;
	if (!logFile.fail())
	{
		DisplayTimestamp(logFile);
		logFile << "Webserv: " << message << std::endl;
	}
}

void	Webserv::DisplayTimestamp(std::ostream& stream)
{
	time_t now = time(NULL);
	struct tm *clock= localtime(&now);

	stream << "[" << 1900 + clock->tm_year;
	stream << std::setfill('0') << std::setw(2) << clock->tm_mon;
	stream << std::setfill('0') << std::setw(2) << clock->tm_mday << "_";
	stream << std::setfill('0') << std::setw(2) << clock->tm_hour;
	stream << std::setfill('0') << std::setw(2) << clock->tm_min;
	stream << std::setfill('0') << std::setw(2) << clock->tm_sec;
	stream << "] ";
}
