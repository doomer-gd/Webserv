/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:21:58 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/18 17:16:07 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"


Parser::Parser(std::string& buffer, const Config& config, Client* client):
	bufferMain(buffer), bufferSize(config.bufferSize), bytesRead(0), client(client)
{
	bufferTemp.reserve(bufferSize);
}

Parser::~Parser(){};

void	Parser::Initialize()
{
	Webserv::Log("Initializing parser");
	std::cout << bufferMain << bytesRead << somethingToReturn << std::endl; //filler
}

int	Parser::Execute()
{
	Webserv::Log("Executing parser");
	if (client != NULL)
		client->GetFd(); //filler for the compiler
	return 0;
}

ClientState	Parser::Exit()
{
	Webserv::Log("Exiting parser");
	return CS_READING_BODY;
}
