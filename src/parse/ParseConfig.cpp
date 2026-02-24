/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 17:40:13 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/24 22:27:41 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main/main.hpp"

int	ConfigParser::ParseConfigFile(const std::string& fileName)
{
	OpenFile(fileName);
	return 0;
}

int	ConfigParser::OpenFile(const std::string& fileName)
{
	fileInput.open(fileName, std::ifstream::in);
	if (fileInput.fail())
		throw Webserv::Except("failed to open configuration file: " + fileName);
	return E_SUCCESS;
}
