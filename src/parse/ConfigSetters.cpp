/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigSetters.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 12:39:45 by ikulik            #+#    #+#             */
/*   Updated: 2026/03/20 12:14:49 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse/ConfigSetters.hpp"
#include "parse/ConfigDefines.hpp"
#include "main/Webserv.hpp"
#include "utils/Codes.hpp"

const std::string g_supported_methods[NUM_SUP_METHODS] = {"GET", "SET", "POST"};

const struct Size g_memory_formats[] = {{'k', 1024},{'m', 1048576}};

const struct Size g_time_formats[] = {{'s', 1}, {'m', 60}, {'h', 3600}, {'d', 86400}, {'M', 2592000}, {'y', 31536000}};

template int ConfigSetters::SetSingleParam<int>(int&, LineArray&, EConfigDict, Verifier, int (*)(const std::string&));
template int ConfigSetters::SetSingleParam<std::string>(std::string&, LineArray&, EConfigDict, Verifier, std::string (*)(const std::string&));
template int ConfigSetters::SetSingleParam<IpPort>(IpPort&, LineArray&, EConfigDict, Verifier, IpPort (*)(const std::string&));


ConfigTimouts::ConfigTimouts():header(DEF_TIMEOUT), body(DEF_TIMEOUT),
	keepAlive(DEF_TIMEOUT), send(DEF_TIMEOUT), general(DEF_TIMEOUT){};

ConfigSetters::ConfigSetters(ConfigMain& config):currentScope(CD_MAIN), currentSetter(NULL)
{
	this->config = &config;
	SetUpScopeHeirarchy();
	SetUpDictionaries();
};

void ConfigSetters::SetUpScopeHeirarchy(void)
{
	scopeHier.AddParent(CD_MAIN, CD_MAIN);
	scopeHier.AddParent(CD_EVENTS, CD_MAIN);
	scopeHier.AddParent(CD_HTTP, CD_MAIN);
	scopeHier.AddParent(CD_SERVER, CD_HTTP);
	scopeHier.AddParent(CD_LOCATION, CD_SERVER);
}

void	ConfigSetters::SetUpDictionaries()
{
	//main
	dicts[CD_MAIN]["client_header_buffer_size"] = &ConfigSetters::SetHeaderBufferSize;
	dicts[CD_MAIN]["client_body_buffer_size"] = &ConfigSetters::SetBodyBufferSize;
	dicts[CD_MAIN]["worker_rlimit_nofile"] = &ConfigSetters::SetFdsMax;
	dicts[CD_MAIN]["error_log"] = &ConfigSetters::SetErrorLog;
	dicts[CD_MAIN]["events"] = &ConfigSetters::SetEvents;
	//events
	dicts[CD_EVENTS]["worker_connections"] = &ConfigSetters::SetMaxConnections;
	//http
	dicts[CD_HTTP]["server"] = &ConfigSetters::SetServer;
	dicts[CD_HTTP]["client_header_timeout"] = &ConfigSetters::SetHeaderTimeout;
	dicts[CD_HTTP]["client_body_timeout"] = &ConfigSetters::SetBodyTimeout;
	dicts[CD_HTTP]["keepalive_timeout"] = &ConfigSetters::SetKeepAliveTimeout;
	dicts[CD_HTTP]["send_timeout"] = &ConfigSetters::SetSendTimeout;
	dicts[CD_HTTP]["common_timeout"] = &ConfigSetters::SetGeneralTimeout;
	//server
	dicts[CD_SERVER]["server_name"] = &ConfigSetters::SetServerName;
	dicts[CD_SERVER]["error_pages"] = &ConfigSetters::SetErrorPages;
	dicts[CD_SERVER]["listen"] = &ConfigSetters::SetListen;
	dicts[CD_SERVER]["client_max_body_size"] = &ConfigSetters::SetMaxBodySize;
	dicts[CD_SERVER]["location"] = &ConfigSetters::SetLocation;
	//location
	dicts[CD_LOCATION]["root"] = &ConfigSetters::SetRoot;
	dicts[CD_LOCATION]["index"] = &ConfigSetters::SetIndex;
	dicts[CD_LOCATION]["autoindex"] = &ConfigSetters::SetAutoindex;
	dicts[CD_LOCATION]["methods"] = &ConfigSetters::SetMethods;
	dicts[CD_LOCATION]["return"] = &ConfigSetters::SetRedirect;
	dicts[CD_LOCATION]["upload_store"] = &ConfigSetters::SetUploadStore;
	dicts[CD_LOCATION]["cgi_redir"] = &ConfigSetters::SetCgi;
}

int	ConfigSetters::SelectSetter(const std::string& nameParameter)
{
	Dictionary::iterator	find;
	find = dicts[currentScope].find(nameParameter);
	if (find == dicts[currentScope].end())
	{
		Webserv::Log("Unrecognized config file variable name: " + nameParameter);
		return E_FAILURE;
	}
	currentSetter = find->second;
	return E_SUCCESS;
}

int	ConfigSetters::SetParameter(LineArray& args)
{
	if (currentSetter == NULL)
		return E_FAILURE;
	return (this->*currentSetter)(args);
}

int	ConfigSetters::VerifySingleParam(LineArray& args, EConfigDict scope, Verifier verify)
{
	if (currentScope != scope || args.size() != 1)
		return E_FAILURE;
	if ((*verify)(args[0]) == false)
		return E_FAILURE;
	return E_SUCCESS;
}

int	ConfigSetters::VerifyMultipleParam(LineArray& args, EConfigDict scope, Verifier verify)
{
	int	size = args.size();
	if (currentScope != scope || size == 0)
		return E_FAILURE;
	for (int i = 0; i < size; i++)
	{
		if ((*verify)(args[i]) == false)
			return E_FAILURE;
	}
	return E_SUCCESS;
}

int	ConfigSetters::SetErrorLog(LineArray& args)
{
	return SetSingleParam(config->logFileName, args, CD_MAIN, VerifyFilePath, ConfigSetters::ConvertFilePath);
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
inline int	ConfigSetters::SetMaxConnections(LineArray& args)
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
	if (result == E_FAILURE)
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
	if (currentServer == NULL)
		return E_FAILURE;
	currentServer->locations.push_back(LocationConfig());
	currentLocation = &(currentServer->locations.back());
	return E_SUCCESS;
}

//location
inline int	ConfigSetters::SetRoot(LineArray& args)
{
	if (currentLocation == NULL)
		return E_FAILURE;
	return SetSingleParam(currentLocation->root, args, CD_LOCATION, VerifyDirectory, ConvertDirectory);
}
inline int	ConfigSetters::SetIndex(LineArray& args)
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
inline int	ConfigSetters::SetMethods(LineArray& args)
{
	if (currentLocation == NULL)
		return E_FAILURE;
	return SetMultipleParam(currentLocation->methods, args, CD_LOCATION, VerifyMethod, ConvertMethod);
}
inline int	ConfigSetters::SetRedirect(LineArray& args)
{
	if (currentLocation == NULL)
		return E_FAILURE;
	return SetSingleParam(currentLocation->redirect, args, CD_LOCATION, VerifyURL, ConvertURL);
}

inline int	ConfigSetters::SetUploadStore(LineArray& args)
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
