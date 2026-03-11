/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfigFile.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 17:40:13 by ikulik            #+#    #+#             */
/*   Updated: 2026/03/06 18:13:01 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parse/ConfigParser.hpp"
#include "../../include/parse/ConfigSetters.hpp"
#include "../../include/main/Webserv.hpp"
#include "../../include/utils/Codes.hpp"

ConfigParser::ConfigParser(): fileStream(fileInput){};

ConfigParser::~ConfigParser(){};

//Parses the config file
int	ConfigParser::ParseConfigFile(ConfigMain& config, const std::string& fileName)
{
	if (OpenFile(fileName) == E_FAILURE)
		return E_FAILURE;
	tokenizer = new ConfigTokenizer();
	setter = new ConfigSetters(config);

	delete tokenizer;
	delete setter;
	return 0;
}

inline int	ConfigParser::OpenFile(const std::string& fileName)
{
	fileInput.open(fileName.c_str(), std::ifstream::in);
	if (fileInput.fail())
	{
		Webserv::Log("failed to open configuration file: " + fileName);
		return E_FAILURE;
	}
	return E_SUCCESS;
}


//Defines correspondence to set params correctly



