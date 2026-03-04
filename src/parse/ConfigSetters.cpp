/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigSetters.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:39:45 by ikulik            #+#    #+#             */
/*   Updated: 2026/03/04 17:49:47 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

ConfigTimouts::ConfigTimouts():header(DEF_TIMEOUT), body(DEF_TIMEOUT),
	keepAlive(DEF_TIMEOUT), send(DEF_TIMEOUT), general(DEF_TIMEOUT){};

ConfigSetters::ConfigSetters():currentScope(CD_MAIN){};

int	ConfigSetters::SetErrorLog(LineArray& args)
{
	return SetSingleParam(config->logFileName, args, CD_MAIN, VerifyDirectory, ConvertDirectory);
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

bool	ConfigSetters::VerifyNumber(const std::string& str)
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

	return SetSingleParam(config->fdsMax, args, CD_MAIN, VerifyNumber, ConvertNumber);
}

int	ConfigSetters::SetHeaderBufferSize(LineArray& args)
{
	return SetSingleParam(config->bufferSize, args, CD_MAIN, VerifyNumber, ConvertSize);
}

int	ConfigSetters::SetBodyBufferSize(LineArray& args)
{
	return SetSingleParam(config->bodyBufferSize, args, CD_MAIN, VerifyNumber, ConvertSize);
}

//events
int	ConfigSetters::SetMaxConnections(LineArray& args)
{
	return SetSingleParam(config->connectionsMax, args, CD_MAIN, VerifyNumber, ConvertNumber);
}

int	ConfigSetters::SetHttp(LineArray& args)
{
	return SetScope(args, CD_HTTP, CD_MAIN);
}

//http

int	ConfigSetters::SetHeaderTimeout(LineArray& args)
{
	return SetSingleParam(config->timeOut.header, args, CD_HTTP, VerifyTime, ConvertTime);
}

int	ConfigSetters::SetBodyTimeout(LineArray& args)
{
	return SetSingleParam(config->timeOut.body, args, CD_HTTP, VerifyTime, ConvertTime);
}

int	ConfigSetters::SetKeepAliveTimeout(LineArray& args)
{
	return SetSingleParam(config->timeOut.keepAlive, args, CD_HTTP, VerifyTime, ConvertTime);
}

int	ConfigSetters::SetSendTimeout(LineArray& args)
{
	return SetSingleParam(config->timeOut.send, args, CD_HTTP, VerifyTime, ConvertTime);
}

int	ConfigSetters::SetGeneralTimeout(LineArray& args)
{
	return SetSingleParam(config->timeOut.general, args, CD_HTTP, VerifyTime, ConvertTime);
}
int	ConfigSetters::SetServer(LineArray& args)
{
	return SetScope(args, CD_SERVER, CD_HTTP);
}
//server
int	ConfigSetters::SetServerName(LineArray& args)
{
	int result;

	if (currentServer == NULL)
		return E_FAILURE;
	result = SetMultipleParam(currentServer->serverNames, args, CD_SERVER, VerifyURL, ConvertURL);
	if (result = E_FAILURE)
		return E_FAILURE;
	currentServer->serverName = currentServer->serverNames[0];
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
	return SetMultipleParam(currentServer->portsArray, args, CD_SERVER, VerifyIP, ConvertIP);
}
int	ConfigSetters::SetMaxBodySize(LineArray& args)
{
	return SetSingleParam(currentServer->clientMaxBodySize, args, CD_SERVER, VerifySize, ConvertSize);
}

//this will take some reading
int	ConfigSetters::SetLocation(LineArray& args);

//location
int	ConfigSetters::SetRoot(LineArray& args)
{
	if (currentLocation == NULL)
		return E_FAILURE;
	return SetSingleParam(currentLocation->root, args, CD_LOCATION, VerifyDirectory, ConvertDirectory);
}
int	ConfigSetters::SetIndex(LineArray& args)
{
	if (currentLocation == NULL)
		return E_FAILURE;
	return SetSingleParam(currentLocation->index, args, CD_LOCATION, VerifyDirectory, ConvertDirectory);
}

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
int	ConfigSetters::SetMethods(LineArray& args)
{
	if (currentLocation == NULL)
		return E_FAILURE;
	return SetMultipleParam(currentLocation->methods, args, CD_LOCATION, VerifyMethod, ConvertMethod);
}
int	ConfigSetters::SetRedirect(LineArray& args);
int	ConfigSetters::SetUploadStore(LineArray& args);
int	ConfigSetters::SetCgiPath(LineArray& args);
