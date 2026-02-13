/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 16:22:01 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/13 17:46:07 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Executer::Executer(std::string& buffer, Command& command): buffer(buffer), command(command){};


void	Executer::Initialize()
{
	std::cout << "Initializing executer" << std::endl;
}

int	Executer::Execute()
{
	std::cout << "Executing executer" << std::endl;
	return 0;
}

ClientState	Executer::Exit()
{
	std::cout << "Exiting executer" << std::endl;
	return CS_SENDING;
}
