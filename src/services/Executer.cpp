/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 16:22:01 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/18 17:15:41 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Executer::Executer(std::string& buffer, Command& command): buffer(buffer), command(command){};

void	Command::DoSomething(void)
{
	Webserv::Log("Command complete!");
}

void	Executer::Initialize()
{
	Webserv::Log("Initializing executer");
}

int	Executer::Execute()
{
	Webserv::Log("Executing executer");
	return 0;
}

ClientState	Executer::Exit()
{
	std::cout << "Exiting executer" << std::endl;
	return CS_SENDING;
}
