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

#include "main.hpp"

volatile sig_atomic_t g_signal = 0;

static void	signalHandler(int signum)
{
	g_signal = signum;
}

int	main(int argc, char** argv)
{
	std::string	configPath = "default.conf";

	if (argc > 2)
		return (Webserv::Exit(E_WRONG_ARGUMENTS));
	if (argc == 2)
		configPath = argv[1];

	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGPIPE, SIG_IGN);

	Webserv::Log("Starting webserv with config: " + configPath);

	try
	{
		ConfigParser	parser;
		Config			config = parser.parse(configPath);

		Webserv::Log("Config loaded: " + toString(config.servers.size())
			+ " server(s), " + toString(config.numSockets) + " port(s)");

		TaskManager	managerMain(config);
		managerMain.InnitializeServer();
		managerMain.StartMainLoop();
	}
	catch (const std::exception& e)
	{
		Webserv::Log(std::string("Config error: ") + e.what());
		return (Webserv::Exit(E_FAILURE));
	}
	return (Webserv::Exit(E_SUCCESS));
}
