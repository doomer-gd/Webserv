/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigSetters.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:39:45 by ikulik            #+#    #+#             */
/*   Updated: 2026/03/05 16:50:10 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

ConfigTimouts::ConfigTimouts():header(DEF_TIMEOUT), body(DEF_TIMEOUT),
	keepAlive(DEF_TIMEOUT), send(DEF_TIMEOUT), general(DEF_TIMEOUT){};

ConfigSetters::ConfigSetters():currentScope(CD_MAIN)
{
	SetUpScopeHeirarchy();
};

void ConfigSetters::SetUpScopeHeirarchy(void)
{
	scopeHier.AddParent(CD_MAIN, CD_MAIN);
	scopeHier.AddParent(CD_EVENTS, CD_MAIN);
	scopeHier.AddParent(CD_HTTP, CD_MAIN);
	scopeHier.AddParent(CD_SERVER, CD_MAIN);
	scopeHier.AddParent(CD_LOCATION, CD_SERVER);
}

int	ConfigSetters::SetErrorLog(LineArray& args)
{
	return SetSingleParam(config->logFileName, args, CD_MAIN, VerifyFilePath, ConvertFilePath);
}


inline int ConfigSetters::SetScope(LineArray& args, EConfigDict scopeNew)
{
	int	parent;

	parent = scopeHier.GetParent(scopeNew);
	if (parent == LOOKUP_FAIL || parent != currentScope)
		return E_FAILURE;
	if (args.back().compare("{") != 0)
		return E_FAILURE;
	currentScope = scopeNew;
	return E_SUCCESS;
}


inline int	ConfigSetters::SetEvents(LineArray& args)
{
	return SetScope(args, CD_EVENTS);
}

inline int	ConfigSetters::SetFdsMax(LineArray& args)
{
	return SetSingleParam(config->fdsMax, args, CD_MAIN, VerifyNumber, ConvertNumber);
}

inline int	ConfigSetters::SetHeaderBufferSize(LineArray& args)
{
	return SetSingleParam(config->bufferSize, args, CD_MAIN, VerifyNumber, ConvertSize);
}

inline int	ConfigSetters::SetBodyBufferSize(LineArray& args)
{
	return SetSingleParam(config->bodyBufferSize, args, CD_MAIN, VerifyNumber, ConvertSize);
}

//events
int	ConfigSetters::SetMaxConnections(LineArray& args)
{
	return SetSingleParam(config->connectionsMax, args, CD_MAIN, VerifyNumber, ConvertNumber);
}

//http
inline int	ConfigSetters::SetHttp(LineArray& args)
{
	return SetScope(args, CD_HTTP);
}

inline int	ConfigSetters::SetHeaderTimeout(LineArray& args)
{
	return SetSingleParam(config->timeOut.header, args, CD_HTTP, VerifyTime, ConvertTime);
}

inline int	ConfigSetters::SetBodyTimeout(LineArray& args)
{
	return SetSingleParam(config->timeOut.body, args, CD_HTTP, VerifyTime, ConvertTime);
}

inline int	ConfigSetters::SetKeepAliveTimeout(LineArray& args)
{
	return SetSingleParam(config->timeOut.keepAlive, args, CD_HTTP, VerifyTime, ConvertTime);
}

inline int	ConfigSetters::SetSendTimeout(LineArray& args)
{
	return SetSingleParam(config->timeOut.send, args, CD_HTTP, VerifyTime, ConvertTime);
}

inline int	ConfigSetters::SetGeneralTimeout(LineArray& args)
{
	return SetSingleParam(config->timeOut.general, args, CD_HTTP, VerifyTime, ConvertTime);
}
inline int	ConfigSetters::SetServer(LineArray& args)
{
	if (SetScope(args, CD_SERVER) == E_FAILURE)
		return E_FAILURE;
	config->servers.push_back(ServerConfig());
	currentServer = &(config->servers.back());
	return E_SUCCESS;
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
	return E_SUCCESS;
}

int	ConfigSetters::SetErrorPages(LineArray& args)
{
	int	size = args.size();
	int	errorCode;

	if (currentScope != CD_SERVER)
		return E_FAILURE;
	if (VerifyFilePath(args[size - 1]) == false)
		return E_FAILURE;
	for (int i = 0; i < size - 1; i++)
	{
		errorCode = VerifyNumber(args[i]);
		if (errorCode == VER_ERROR)
			return E_FAILURE;
		currentServer->errorPages[errorCode] = args[size - 1];
	}
	return E_SUCCESS;
}

inline int	ConfigSetters::SetListen(LineArray& args)
{
	if (currentServer == NULL)
		return E_FAILURE;
	return SetMultipleParam(currentServer->portsArray, args, CD_SERVER, VerifyIP, ConvertIP);
}
inline int	ConfigSetters::SetMaxBodySize(LineArray& args)
{
	if (currentServer == NULL)
		return E_FAILURE;
	return SetSingleParam(currentServer->clientMaxBodySize, args, CD_SERVER, VerifySize, ConvertSize);
}

int	ConfigSetters::SetLocation(LineArray& args)
{
	if (currentScope != CD_SERVER || args.size() != 2)
		return E_FAILURE;
	if (VerifyDirectory(args[0]) == false)
		return E_FAILURE;
	if (args[1].compare("{") != 0)
		return E_FAILURE;
	return E_SUCCESS;
}

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
	return SetSingleParam(currentLocation->index, args, CD_LOCATION, VerifyFilePath, ConvertFilePath);
}

int	ConfigSetters::SetAutoindex(LineArray& args)
{
	if (currentLocation == NULL)
		return E_FAILURE;
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
int	ConfigSetters::SetRedirect(LineArray& args)
{
	if (currentLocation == NULL)
		return E_FAILURE;
	return SetSingleParam(currentLocation->redirect, args, CD_LOCATION, VerifyURL, ConvertURL);
}

int	ConfigSetters::SetUploadStore(LineArray& args)
{
	if (currentLocation == NULL)
		return E_FAILURE;
	return SetSingleParam(currentLocation->uploadStore, args, CD_LOCATION, VerifyDirectory, ConvertDirectory);
}
int	ConfigSetters::SetCgi(LineArray& args)
{
	if (currentLocation == NULL || args.size() != 2
		|| currentScope != CD_LOCATION)
		return E_FAILURE;
	if (!VerifyExtension(args[0]) || !VerifyDirectory(args[1]))
		return E_FAILURE;
	currentLocation->cgiExtension = args[0];
	currentLocation->cgiPath = args[1];
	currentLocation->cgiRedirs[args[0]] = args[1];
	return E_SUCCESS;
}
