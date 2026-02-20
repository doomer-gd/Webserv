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

int	main(int argc, char** argv)
{
	Webserv		server;
	TaskManager	managerMain;

	if (argc != 2)
		return (Webserv::Exit(E_WRONG_ARGUMENTS));
	Webserv::Log("Welcome to Webserver, config file: " + std::string(argv[1]));
	Webserv::exitCode_ = managerMain.InnitializeServer();
	if (Webserv::exitCode_ != E_SUCCESS)
		return (Webserv::Exit(Webserv::exitCode_));
	managerMain.StartMainLoop();
	return (Webserv::Exit(E_SUCCESS));
}
