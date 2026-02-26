/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 17:40:13 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/26 20:31:36 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../main/main.hpp"

//Parses the config file
int	ConfigParser::GetConfig(ConfigMain& config, const std::string& fileName)
{
	OpenFile(fileName);
	return 0;
}

inline int	ConfigParser::OpenFile(const std::string& fileName)
{
	fileInput.open(fileName, std::ifstream::in);
	if (fileInput.fail())
		throw Webserv::Except("failed to open configuration file: " + fileName);
	return E_SUCCESS;
}

//Defines correspondence to set params correctly
void	ConfigParser::SetUpDictionaries()
{
	//main
	dicts[CD_MAIN]["client_header_buffer_size"] = &ConfigSetters::SetHeaderBufferSize;
	dicts[CD_MAIN]["client_body_buffer_size"] = &ConfigSetters::SetBodyBufferSize;
	dicts[CD_MAIN]["worker_rlimit_nofile"] = &ConfigSetters::SetFdsMax;
	dicts[CD_MAIN]["error_log"] = &ConfigSetters::SetErrorLog;
	dicts[CD_MAIN]["events"] = &ConfigSetters::SetEvents;
	dicts[CD_MAIN]["user"] = &ConfigSetters::SetUserGroup;
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
	dicts[CD_LOCATION]["include"] = &ConfigSetters::SetCgiPath;
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