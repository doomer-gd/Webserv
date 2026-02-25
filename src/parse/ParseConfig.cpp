/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 17:40:13 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/25 20:11:15 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main/main.hpp"

int	ConfigParser::GetConfig(Config& config, const std::string& fileName)
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

void	ConfigParser::SetUpDictionaries()
{
	// void	SetWorkerProcs(std::string& value, void* content)
	// {
	// 	if (VerifyNumber(value) == false)
	// 		throw Webserv::Except("configuration file error");
	// };
}

bool	ConfigParser::VerifyNumber(const std::string& str)
{
	std::string::const_iterator	iter = str.begin();
	for (;iter != str.end();iter++)
	{
		if (isdigit(*iter) == 0)
			return false;
	}
	return true;
}