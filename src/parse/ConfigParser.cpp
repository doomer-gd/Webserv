/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtrofyme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 00:00:00 by vtrofyme          #+#    #+#             */
/*   Updated: 2026/03/16 00:00:00 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include <iostream>
#include <cstdlib>
#include <set>

ConfigParser::ConfigParser() : pos(0) {}
ConfigParser::~ConfigParser() {}

void	ConfigParser::tokenize(const std::string& content)
{
	std::string	current;

	tokens.clear();
	for (size_t i = 0; i < content.size(); i++)
	{
		char c = content[i];
		if (c == '#')
		{
			if (!current.empty())
			{
				tokens.push_back(current);
				current.clear();
			}
			while (i < content.size() && content[i] != '\n')
				i++;
		}
		else if (c == '{' || c == '}' || c == ';')
		{
			if (!current.empty())
			{
				tokens.push_back(current);
				current.clear();
			}
			tokens.push_back(std::string(1, c));
		}
		else if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
		{
			if (!current.empty())
			{
				tokens.push_back(current);
				current.clear();
			}
		}
		else
			current += c;
	}
	if (!current.empty())
		tokens.push_back(current);
}

bool	ConfigParser::hasMore() const
{
	return pos < tokens.size();
}

std::string	ConfigParser::peek() const
{
	if (pos >= tokens.size())
		throw std::runtime_error("Config: unexpected end of file");
	return tokens[pos];
}

std::string	ConfigParser::next()
{
	if (pos >= tokens.size())
		throw std::runtime_error("Config: unexpected end of file");
	return tokens[pos++];
}

void	ConfigParser::expect(const std::string& expected)
{
	std::string	tok = next();
	if (tok != expected)
		throw std::runtime_error("Config: expected '" + expected + "', got '" + tok + "'");
}

size_t	ConfigParser::parseSize(const std::string& sizeStr)
{
	if (sizeStr.empty())
		return 0;

	char	suffix = sizeStr[sizeStr.size() - 1];
	std::string	numStr = sizeStr;

	if (suffix == 'K' || suffix == 'k')
	{
		numStr = sizeStr.substr(0, sizeStr.size() - 1);
		return static_cast<size_t>(atol(numStr.c_str())) * 1024;
	}
	else if (suffix == 'M' || suffix == 'm')
	{
		numStr = sizeStr.substr(0, sizeStr.size() - 1);
		return static_cast<size_t>(atol(numStr.c_str())) * 1024 * 1024;
	}
	else if (suffix == 'G' || suffix == 'g')
	{
		numStr = sizeStr.substr(0, sizeStr.size() - 1);
		return static_cast<size_t>(atol(numStr.c_str())) * 1024 * 1024 * 1024;
	}
	return static_cast<size_t>(atol(sizeStr.c_str()));
}

LocationConfig	ConfigParser::parseLocationBlock()
{
	LocationConfig	loc;

	loc.path = next();
	expect("{");

	while (hasMore() && peek() != "}")
	{
		std::string	directive = next();

		if (directive == "root")
		{
			loc.root = next();
			if (hasMore() && peek() == ";")
				next();
		}
		else if (directive == "index")
		{
			loc.index = next();
			if (hasMore() && peek() == ";")
				next();
		}
		else if (directive == "autoindex")
		{
			std::string	val = next();
			loc.autoindex = (val == "on");
			if (hasMore() && peek() == ";")
				next();
		}
		else if (directive == "methods")
		{
			loc.methods.clear();
			while (hasMore() && peek() != ";" && peek() != "}"
				&& peek() != "root" && peek() != "index"
				&& peek() != "autoindex" && peek() != "methods"
				&& peek() != "redirect" && peek() != "upload_store"
				&& peek() != "cgi_extension" && peek() != "cgi_path"
				&& peek() != "location")
			{
				std::string	method = next();
				loc.methods.insert(method);
			}
			if (hasMore() && peek() == ";")
				next();
		}
		else if (directive == "redirect")
		{
			loc.redirect = next();
			if (hasMore() && peek() == ";")
				next();
		}
		else if (directive == "upload_store")
		{
			loc.uploadStore = next();
			if (hasMore() && peek() == ";")
				next();
		}
		else if (directive == "cgi_extension")
		{
			loc.cgiExtension = next();
			if (hasMore() && peek() == ";")
				next();
		}
		else if (directive == "cgi_path")
		{
			loc.cgiPath = next();
			if (hasMore() && peek() == ";")
				next();
		}
		else
			throw std::runtime_error("Config: unknown location directive '" + directive + "'");
	}

	expect("}");
	return loc;
}

ServerConfig	ConfigParser::parseServerBlock()
{
	ServerConfig	srv;

	expect("{");
	srv.locations.clear();

	while (hasMore() && peek() != "}")
	{
		std::string	directive = next();

		if (directive == "listen")
		{
			std::string	portStr = next();
			srv.port = atoi(portStr.c_str());
			if (srv.port <= 0 || srv.port > 65535)
				throw std::runtime_error("Config: invalid port '" + portStr + "'");
			if (hasMore() && peek() == ";")
				next();
		}
		else if (directive == "server_name")
		{
			srv.serverName = next();
			if (hasMore() && peek() == ";")
				next();
		}
		else if (directive == "client_max_body_size")
		{
			std::string	sizeStr = next();
			srv.clientMaxBodySize = parseSize(sizeStr);
			if (hasMore() && peek() == ";")
				next();
		}
		else if (directive == "error_page")
		{
			std::string	codeStr = next();
			std::string	pagePath = next();
			int	code = atoi(codeStr.c_str());
			srv.errorPages[code] = pagePath;
			if (hasMore() && peek() == ";")
				next();
		}
		else if (directive == "location")
		{
			LocationConfig	loc = parseLocationBlock();
			srv.locations.push_back(loc);
		}
		else
			throw std::runtime_error("Config: unknown server directive '" + directive + "'");
	}

	expect("}");

	if (srv.locations.empty())
		throw std::runtime_error("Config: server block must have at least one location");

	return srv;
}

Config	ConfigParser::parse(const std::string& filepath)
{
	std::ifstream	file(filepath.c_str());
	if (!file.is_open())
		throw std::runtime_error("Config: cannot open file '" + filepath + "'");

	std::stringstream	ss;
	ss << file.rdbuf();
	file.close();

	std::string	content = ss.str();
	tokenize(content);
	pos = 0;

	Config	config;
	config.servers.clear();
	config.socketPorts.clear();

	while (hasMore())
	{
		std::string	tok = next();
		if (tok == "server")
		{
			ServerConfig	srv = parseServerBlock();
			config.servers.push_back(srv);
		}
		else
			throw std::runtime_error("Config: expected 'server', got '" + tok + "'");
	}

	if (config.servers.empty())
		throw std::runtime_error("Config: no server blocks defined");

	std::set<int>	uniquePorts;
	for (size_t i = 0; i < config.servers.size(); i++)
		uniquePorts.insert(config.servers[i].port);

	for (std::set<int>::iterator it = uniquePorts.begin(); it != uniquePorts.end(); ++it)
		config.socketPorts.push_back(*it);

	config.numSockets = config.socketPorts.size();
	return config;
}
