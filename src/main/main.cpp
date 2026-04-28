/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 14:44:12 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/13 18:07:23 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main/main.hpp"

extern volatile sig_atomic_t g_signal;

static void	signalHandler(int signum)
{
	g_signal = signum;
}

int	main(int argc, char** argv)
{
	const char*	configPath = "default.conf";

	if (argc > 2)
		return (Webserv::Exit(E_WRONG_ARGUMENTS));
	if (argc == 2)
		configPath = argv[1];

	Webserv::Log("Starting webserv with config: " + std::string(configPath));

	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGPIPE, SIG_IGN);
	try
	{
		ConfigParser	parser;
		ConfigMain		config;

		if (parser.ParseConfigFile(config, configPath) != E_SUCCESS)
		{
			Webserv::Log("Failed to parse config file: " + std::string(configPath));
			return (Webserv::Exit(E_FAILURE));
		}

		Webserv::Log("Config loaded: " + toString(config.servers.size())
			+ " server(s), " + toString(config.numSockets) + " port(s)");

		TaskManager	managerMain(config);
		managerMain.InnitializeServer();
		managerMain.StartMainLoop();
	}
	catch (const std::exception& e)
	{
		Webserv::Log(std::string("Server error: ") + e.what());
		return (Webserv::Exit(E_FAILURE));
	}
	return (Webserv::Exit(E_SUCCESS));
}
