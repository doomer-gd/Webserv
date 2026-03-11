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

#include "../../include/main/main.hpp"

int	main(int argc, char** argv)
{
	Webserv			server;
	ConfigMain*		config;
	TaskManager*	managerMain;

	if (argc != 2)
		return (Webserv::Exit(E_WRONG_ARGUMENTS));
	Webserv::Log("Welcome to Webserver, configuration file: " + std::string(argv[1]));
	if (server.Innitialize(argv[1]) == E_FAILURE)
		return (EXIT_FAILURE);
	config = server.GetConfig();
	managerMain = server.GetTaskManager();
	managerMain->StartMainLoop();
	(void)config; //might need it later
	return (Webserv::Exit(E_SUCCESS));
}
