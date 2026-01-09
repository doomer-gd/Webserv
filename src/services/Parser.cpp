/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:21:58 by ikulik            #+#    #+#             */
/*   Updated: 2026/01/09 16:37:22 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"


Parser::Parser(std::string& buffer, Config& config, Client* client):
	bufferMain(buffer), bufferSize(config.bufferSize), bytesRead(0), client(client)
{
	bufferTemp = std::string(bufferSize, 0);
}
void	Parser::Initialize()
{
	std::cout << "Initializing parser" << std::endl;
}
int	Parser::Execute()
{
	std::cout << "Executing parser" << std::endl;
	return 0;
}

ClientState	Parser::Exit()
{
	std::cout << "Exiting parser" << std::endl;
	return CS_READING_BODY;
}
