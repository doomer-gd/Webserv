/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigSetters.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:39:45 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/28 22:26:33 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

ConfigTimouts::ConfigTimouts():header(DEF_TIMEOUT), body(DEF_TIMEOUT),
	keepAlive(DEF_TIMEOUT), send(DEF_TIMEOUT), general(DEF_TIMEOUT){};

ConfigSetters::ConfigSetters():currentScope(CD_MAIN){};

int	ConfigSetters::SetErrorLog(LineArray& args)
{
	if (args.size() != 1)
		return E_FAILURE;
	(*config).logFileName = args[0];
	return E_SUCCESS;
}

int ConfigSetters::SetInt(int& var, LineArray& args, EConfigDict scope, int (*setter)(const std::string&))
{
	int	result;

	if (currentScope != scope || args.size() != 1)
		return E_FAILURE;
	result = (*setter)(args[0]);
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

int	ConfigSetters::VerifyNumber(const std::string& str)
{
	std::string::const_iterator	iter = str.begin();
	for (;iter != str.end();iter++)
	{
		if (isdigit(*iter) == 0)
			return false;
	}
	return true;
}

int	ConfigSetters::SetEvents(LineArray& args)
{
	return SetScope(args, CD_EVENTS, CD_MAIN);
}

int	ConfigSetters::SetFdsMax(LineArray& args)
{
	return SetInt(config->fdsMax, args, CD_MAIN, VerifyNumber);
}

int	ConfigSetters::SetHeaderBufferSize(LineArray& args)
{
	return	SetInt(config->bufferSize, args, CD_MAIN, VerifySize);
}

int	ConfigSetters::SetBodyBufferSize(LineArray& args)
{
	return	SetInt(config->bodyBufferSize, args, CD_MAIN, VerifySize);
}

//events
int	ConfigSetters::SetMaxConnections(LineArray& args)
{
	return SetInt(config->connectionsMax, args, CD_MAIN, VerifyNumber);
}

int	ConfigSetters::SetHttp(LineArray& args)
{
	return SetScope(args, CD_HTTP, CD_MAIN);
}

//http
int ConfigSetters::SetTimeout(int& timeout, LineArray& args, EConfigDict scope)
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
int	ConfigSetters::SetHeaderTimeout(LineArray& args)
{
	return SetInt(config->timeOut.header, args, CD_HTTP, VerifyTime);
}

int	ConfigSetters::SetBodyTimeout(LineArray& args)
{
	return SetInt(config->timeOut.body, args, CD_HTTP, VerifyTime);
}

int	ConfigSetters::SetKeepAliveTimeout(LineArray& args)
{
	return SetInt(config->timeOut.keepAlive, args, CD_HTTP, VerifyTime);
}

int	ConfigSetters::SetSendTimeout(LineArray& args)
{
	return SetInt(config->timeOut.send, args, CD_HTTP, VerifyTime);
}

int	ConfigSetters::SetGeneralTimeout(LineArray& args)
{
	return SetInt(config->timeOut.general, args, CD_HTTP, VerifyTime);
}
int	ConfigSetters::SetServer(LineArray& args)
{
	return SetScope(args, CD_SERVER, CD_HTTP);
}
//server
int	ConfigSetters::SetServerName(LineArray& args)
{
	int	size = args.size();

	if (currentScope != CD_SERVER)
		return E_FAILURE;
	for (int i = 0; i < size; i++)
	{
		if (VerifyURL(args[i]) != E_SUCCESS)
			return E_FAILURE;
	}
	currentServer->serverNames = args;
	return E_SUCCESS;
}

int	ConfigSetters::SetErrorPages(LineArray& args)
{
	int	size = args.size();
	int	errorCode;

	if (currentScope != CD_SERVER)
		return E_FAILURE;
	if (VerifyDirectory(args[size - 1]) == E_SUCCESS)
		return E_FAILURE;
	for (int i = 0; i < size - 1; i++)
	{
		errorCode = VerifyNumber(args[i]);
		if (errorCode == -1)
			return E_FAILURE;
		currentServer->errorPages[errorCode] = args[size - 1];
	}
	return E_SUCCESS;
}

int	ConfigSetters::SetListen(LineArray& args)
{
	IpPort	nextAddress;

	if (currentScope != CD_SERVER || args.size() != 1)
		return E_FAILURE;
	nextAddress = VerifyIP(args[0]);
	if (nextAddress.second == -1)
		return E_FAILURE;
	(*currentServer).portsArray.push_back(nextAddress);
	return E_SUCCESS;
}
int	ConfigSetters::SetMaxBodySize(LineArray& args)
{
	return SetInt(currentServer->clientMaxBodySize, args, CD_SERVER, VerifySize);
}

int	ConfigSetters::SetUri(std::string& var, LineArray& args, EConfigDict scope)
{
	if (currentScope != scope || args.size() != 1)
		return E_FAILURE;
	if (VerifyDirectory(args[0]) == E_FAILURE)
		return E_FAILURE;
	var = args[0];
	return E_SUCCESS;
}


//this will take some reading
int	ConfigSetters::SetLocation(LineArray& args);

//location
int	ConfigSetters::SetRoot(LineArray& args)
{
	return SetUri(currentLocation->root, args, CD_LOCATION);
}
int	ConfigSetters::SetIndex(LineArray& args){};

int	ConfigSetters::SetAutoindex(LineArray& args)
{
	if (currentScope != CD_LOCATION || args.size() != 1)
		return E_FAILURE;
	if (args[0].compare("on") == 0)
		currentLocation->autoindex = true;
	else if (args[0].compare("off") == 0)
		currentLocation->autoindex = false;
	else
		return E_FAILURE;
	return E_SUCCESS;
}
int	ConfigSetters::SetMethods(LineArray& args);
int	ConfigSetters::SetRedirect(LineArray& args);
int	ConfigSetters::SetUploadStore(LineArray& args);
int	ConfigSetters::SetCgiPath(LineArray& args);
