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
	socketPorts(std::vector<int>(DEF_NUM_SOCKETS, DEF_PORT))
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

Webserv::Webserv(): confParser(NULL), config(NULL), managerMain(NULL){};

Webserv::~Webserv()
{
	safeDelete(&confParser);
	safeDelete(&config);
	safeDelete(&managerMain);
};

int	Webserv::Innitialize(const char* fileNameConf)
{
	try
	{
		confParser = new ConfigParser();
		config = new ConfigMain();
		if (confParser->ParseConfigFile(*config, fileNameConf) == E_FAILURE)
			return E_FAILURE;
		managerMain = new TaskManager(*config);
	}
	catch(const std::exception& e)
	{
		Webserv::Log(e.what());
		return E_FAILURE;
	}
	if (managerMain->InnitializeServer() == E_FAILURE)
		return E_FAILURE;
	safeDelete(&confParser);
	return E_SUCCESS;
}

ConfigMain*	Webserv::GetConfig(void) const
{
	return config;
}

TaskManager*	Webserv::GetTaskManager(void) const
{
	return managerMain;
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
