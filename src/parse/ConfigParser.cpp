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

#include "parse/ConfigParser.hpp"
#include "parse/ConfigSetters.hpp"
#include "parse/ConfigTokenizer.hpp"
#include "parse/ConfigDefines.hpp"
#include "main/Webserv.hpp"
#include "utils/Basics.hpp"
#include "utils/Codes.hpp"

#define CHECK_ERROR(expr, error, exitcode) \
    do { if ((expr) == (error)) return (exitcode); } while(0)

ConfigParser::ConfigParser(): fileStream(fileInput){};

ConfigParser::~ConfigParser(){};

//Parses the config file
int	ConfigParser::ParseConfigFile(ConfigMain& config, const char* fileName)
{
	LineArray	args;
	int			status;

	CHECK_ERROR(OpenFile(fileName), E_FAILURE, E_FAILURE);
	config.servers.clear();
	config.socketPorts.clear();
	tokenizer = new ConfigTokenizer();
	setter = new ConfigSetters(config);
	status = tokenizer->GetNextToken(fileStream, buffer);
	while (status == EXECUTING)
	{
		CHECK_ERROR(setter->SelectSetter(buffer), E_FAILURE, ExitParser(E_FAILURE));
		if (buffer.compare("}") == 0)
			fileInput.unget();
		CHECK_ERROR(GetArguments(args), ERROR, ExitParser(E_FAILURE));
		CHECK_ERROR(setter->SetParameter(args), E_FAILURE, ExitParser(E_FAILURE));
		status = tokenizer->GetNextToken(fileStream, buffer);
	}
	if (status == ERROR || buffer.size() != 0)
		return ExitParser(E_FAILURE);
	Webserv::Log("Config loaded: " + toString(config.servers.size())
		+ " server(s), " + toString(config.numSockets) + " port(s)");
	return ExitParser(E_SUCCESS);
}

int	ConfigParser::GetArguments(LineArray& args)
{
	StateStatus	stateToken;

	args.clear();
	stateToken = tokenizer->GetNextToken(fileStream, buffer);
	while (stateToken == EXECUTING)
	{
		args.push_back(buffer);
		if (IsEndingArgument(buffer))
			return EXECUTING;
		stateToken = tokenizer->GetNextToken(fileStream, buffer);
	}
	return stateToken;
}

bool	ConfigParser::IsEndingArgument(const std::string& arg)
{
	const static std::string	enderChars = CONF_TOKEN_ENDERS;

	if (arg.size() == 1 && enderChars.find(arg[0]) != std::string::npos)
		return true;
	return false;
}


int	ConfigParser::ExitParser(int exitCode)
{
	safeDelete(&tokenizer);
	safeDelete(&setter);
	return exitCode;
}

inline int	ConfigParser::OpenFile(const std::string& fileName)
{
	fileInput.open(fileName.c_str(), std::ifstream::in);
	if (fileInput.fail())
	{
		Webserv::Log("Failed to open configuration file: " + fileName);
		return E_FAILURE;
	}
	return E_SUCCESS;
}

const std::string&	ConfigParser::GetErrorLine(void) const
{
	return buffer;
}

//Defines correspondence to set params correctly



