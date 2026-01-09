/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:41:43 by ikulik            #+#    #+#             */
/*   Updated: 2026/01/09 17:37:43 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

Reader::Reader(std::string& buffer): buffer(buffer){};


void	Parser::Initialize()
{
	std::cout << "Initializing reader" << std::endl;
}
int	Parser::Execute()
{
	std::cout << "Executing reader" << std::endl;
	return 0;
}

ClientState	Parser::Exit()
{
	std::cout << "Exiting reader" << std::endl;
	return CS_EXEC_REQUEST;
}
