/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:21:58 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/13 17:13:56 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"


Parser::Parser(std::string& buffer, const Config& config, Client* client):
	bufferMain(buffer), bufferSize(config.bufferSize), bytesRead(0), client(client)
{
	bufferTemp.reserve(bufferSize);
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
