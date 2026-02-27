/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigSetters.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:39:45 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/27 18:48:54 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

ConfigTimouts::ConfigTimouts():header(DEF_TIMEOUT), body(DEF_TIMEOUT),
	keepAlive(DEF_TIMEOUT), send(DEF_TIMEOUT), general(DEF_TIMEOUT){};

ConfigSetters::ConfigSetters():currentScope(CD_MAIN){};

int	ConfigSetters::SetErrorLog(std::vector<std::string>& args)
{
	if (args.size() != 1)
		return E_FAILURE;
	(*config).logFileName = args[0];
	return E_SUCCESS;
}

int ConfigSetters::SetInt(int& var, LineArray& args, EConfigDict scope, int (ConfigSetters::*setter)(const std::string&))
{
	int	result;

	if (currentScope != scope || args.size() != 1)
		return E_FAILURE;
	result = (this->*setter)(args[0]);
	if (result == -1)
		return E_FAILURE;
	var = result;
	return E_SUCCESS;
}


inline int ConfigSetters::SetScope(LineArray& args, EConfigDict scopeNew, EConfigDict scopeParent)
{
	if (currentScope != scopeParent)
		return E_FAILURE;
	if (args.size() != 1 || args[0].compare("{") != 0)
		return E_FAILURE;
	currentScope = scopeNew;
	return E_SUCCESS;
}

int	ConfigSetters::SetEvents(std::vector<std::string>& args)
{
	return SetScope(args, CD_EVENTS, CD_MAIN);
}

int	ConfigSetters::SetFdsMax(std::vector<std::string>& args)
{
	return SetInt((*config).fdsMax, args, CD_MAIN, &VerifyNumber)
}

int	ConfigSetters::SetHeaderBufferSize(std::vector<std::string>& args)
{
	int	result;

	if (currentScope != CD_MAIN || args.size() != 1)
		return E_FAILURE;
	result = ConfigParser::VerifySize(args[0]);
	if (result == -1)
		return E_FAILURE;
	(*config).bufferSize = result;
	return E_SUCCESS;
}

int	ConfigSetters::SetBodyBufferSize(std::vector<std::string>& args)
{
	int	result;

	if (currentScope != CD_MAIN || args.size() != 1)
		return E_FAILURE;
	result = ConfigParser::VerifySize(args[0]);
	if (result == -1)
		return E_FAILURE;
	(*config).bodyBufferSize = result;
	return E_SUCCESS;
}

//events
int	ConfigSetters::SetMaxConnections(std::vector<std::string>& args)
{
	int	result;

	if (currentScope != CD_EVENTS || args.size() != 1)
		return E_FAILURE;
	result = ConfigParser::VerifyNumber(args[0]);
	if (result == -1)
		return E_FAILURE;
	(*config).connectionsMax = result;
	return E_SUCCESS;
}

int	ConfigSetters::SetHttp(std::vector<std::string>& args)
{
	int size = args.size();

	if (currentScope == CD_MAIN)
	{
		if (size != 1 || args[size - 1].compare("{") != 0)
			return E_FAILURE;
		currentScope = CD_HTTP;
		return E_SUCCESS;
	}
	return E_FAILURE;
}

//http
int ConfigSetters::SetTimeout(int& timeout, std::vector<std::string>& args, EConfigDict scope)
{
	int	result;

	if (currentScope != scope || args.size() != 1)
		return E_FAILURE;
	result = ConfigParser::VerifyTime(args[0]);
	if (result == -1)
		return E_FAILURE;
	timeout = result;
	return E_SUCCESS;
}
int	ConfigSetters::SetHeaderTimeout(std::vector<std::string>& args)
{
	return SetTimeout((*config).timeOut.header, args, CD_HTTP);
}

int	ConfigSetters::SetBodyTimeout(std::vector<std::string>& args)
{
	return SetTimeout((*config).timeOut.body, args, CD_HTTP);
}

int	ConfigSetters::SetKeepAliveTimeout(std::vector<std::string>& args)
{
	return SetTimeout((*config).timeOut.keepAlive, args, CD_HTTP);
}

int	ConfigSetters::SetSendTimeout(std::vector<std::string>& args)
{
	return SetTimeout((*config).timeOut.send, args, CD_HTTP);
}

int	ConfigSetters::SetGeneralTimeout(std::vector<std::string>& args)
{
	return SetTimeout((*config).timeOut.general, args, CD_HTTP);
}
int	ConfigSetters::SetServer(std::vector<std::string>& args)
{
	if (currentScope != CD_HTTP || args.size() != 1)
		return E_FAILURE;
	if (args[0].compare("{") != 0)
		return E_FAILURE;
	(*config).servers.push_back(ServerConfig());
	currentServer = &((*config).servers.back());
	currentScope = CD_SERVER;
	return E_SUCCESS;
}
//server
int	ConfigSetters::SetServerName(std::vector<std::string>& args)
{
	int	size = args.size();

	if (currentScope != CD_SERVER)
		return E_FAILURE;
	for (int i = 0; i < size; i++)
	{
		if (ConfigParser::VerifyURL(args[i]) != E_SUCCESS)
			return E_FAILURE;
	}
	(*currentServer).serverNames = args;
	return E_SUCCESS;
}

int	ConfigSetters::SetErrorPages(std::vector<std::string>& args)
{
	int	size = args.size();
	int	errorCode;

	if (currentScope != CD_SERVER)
		return E_FAILURE;
	if (ConfigParser::VerifyDirectory(args[size - 1]) == E_SUCCESS)
		return E_FAILURE;
	for (int i = 0; i < size - 1; i++)
	{
		errorCode = ConfigParser::VerifyNumber(args[i]);
		if (errorCode == -1)
			return E_FAILURE;
		(*currentServer).errorPages[errorCode] = args[size - 1];
	}
	return E_SUCCESS;
}

int	ConfigSetters::SetListen(std::vector<std::string>& args)
{
	IpPort	nextAddress;

	if (currentScope != CD_SERVER || args.size() != 1)
		return E_FAILURE;
	nextAddress = ConfigParser::VerifyIP(args[0]);
	if (nextAddress.second == -1)
		return E_FAILURE;
	(*currentServer).portsArray.push_back(nextAddress);
	return E_SUCCESS;
}
int	ConfigSetters::SetMaxBodySize(std::vector<std::string>& args)
{
	int	result;

	if (currentScope != CD_SERVER || args.size() != 1)
		return E_FAILURE;
	result = ConfigParser::VerifySize(args[0]);
	if (result == -1)
		return E_FAILURE;
	(*currentServer).clientMaxBodySize = result;
	return E_SUCCESS;
}

int	ConfigSetters::SetUri(std::string& var, std::vector<std::string>& args, EConfigDict scope)
{
	if (currentScope != scope || args.size() != 1)
		return E_FAILURE;
	if (ConfigParser::VerifyDirectory(args[0]) == E_FAILURE)
		return E_FAILURE;
	var = args[0];
	return E_SUCCESS;
}


//this will take some reading
int	ConfigSetters::SetLocation(std::vector<std::string>& args);

//location
int	ConfigSetters::SetRoot(std::vector<std::string>& args)
{
	return SetUri((*currentLocation).root, args, CD_LOCATION);
}
int	ConfigSetters::SetIndex(std::vector<std::string>& args);
int	ConfigSetters::SetAutoindex(std::vector<std::string>& args);
int	ConfigSetters::SetMethods(std::vector<std::string>& args);
int	ConfigSetters::SetRedirect(std::vector<std::string>& args);
int	ConfigSetters::SetUploadStore(std::vector<std::string>& args);
int	ConfigSetters::SetCgiPath(std::vector<std::string>& args);
