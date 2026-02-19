/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:41:43 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/18 17:18:00 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Reader::Reader(std::string& buffer): buffer(buffer){};

Reader::~Reader(){};

void	Reader::Initialize()
{
	Webserv::Log("Initializing reader");
}
int	Reader::Execute()
{
	Webserv::Log("Executing reader");
	return 0;
}

ClientState	Reader::Exit()
{
	Webserv::Log("Exiting reader");
	return CS_EXEC_REQUEST;
}
