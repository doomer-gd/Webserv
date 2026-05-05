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

#define CHECK_OK(expr) \
	do { int result = (expr); if ((result) != E_SUCCESS) return (result); } while(0)

volatile sig_atomic_t g_signal = 0;

ConfigMain::ConfigMain():
	bufferSize(DEF_BUFFER_SIZE),
	bodyBufferSize(DEF_BUFFER_SIZE),
	numSockets(DEF_NUM_SOCKETS),
	connectionsMax(DEF_MAX_CONNS),
	fdsMax(DEF_MAX_CONNS),
	socketPorts(std::vector<IpPort>(DEF_NUM_SOCKETS, IpPort(INADDR_ANY, DEF_PORT)))
{};

const char*		Webserv::configPath = "default.conf";
ConfigMain*		Webserv::config = NULL;
TaskManager*	Webserv::managerMain = NULL;
std::ostream&	Webserv::logStream = std::cerr;
std::fstream	Webserv::logFile;

Webserv::Webserv(){};

Webserv::~Webserv(){};

static void	signalHandler(int signum)
{
	g_signal = signum;
}

void	Webserv::HandleSignals( void )
{
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGPIPE, SIG_IGN);
}

int	Webserv::CheckArguments(int argc, char** argv)
{
	if (argc > 2)
		return (E_WRONG_ARGUMENTS);
	if (argc == 2)
		configPath = argv[1];

	Webserv::Log("Starting webserv with config: " + std::string(configPath));
	return E_SUCCESS;
}

int	Webserv::ReadConfig( void )
{
	try
	{
		if (config != NULL)
			return E_SUCCESS;
		config = new ConfigMain();
		ConfigParser	parser;

		if (parser.ParseConfigFile(*config, configPath) != E_SUCCESS)
		{
			Webserv::Log("Failed to parse config file: " + std::string(configPath));
			return (Webserv::Exit(E_FAILURE));
		}

		managerMain = new TaskManager(*config);
		Webserv::OpenLogFile(config->logFileName.c_str());
		Webserv::Log("Config loaded: " + toString(config->servers.size())
			+ " server(s), " + toString(config->numSockets) + " port(s)");
		
	}
	catch (const std::exception& e)
	{
		Webserv::Log(std::string("Server error: ") + e.what());
		return (E_FAILURE);
	}
	return E_SUCCESS;
}

int	Webserv::StartServer( void )
{
	CHECK_OK(managerMain->InnitializeServer());
	return managerMain->StartMainLoop();
}


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
	if (errorCode >= 0 && errorCode < NUM_ERRORS)
		Webserv::Log(g_errorMessage[errorCode]);
	logFile.close();
	safeDelete(&config);
	safeDelete(&managerMain);
	return (errorCode);
}

void	Webserv::Log(const std::string& message)
{
	DisplayTimestamp(logStream);
	logStream << "Webserv: " << message << std::endl;
	if (logFile.is_open() && !logFile.fail())
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
