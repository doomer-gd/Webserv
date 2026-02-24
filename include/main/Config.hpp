/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtrofyme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 10:22:08 by vtrofyme          #+#    #+#             */
/*   Updated: 2026/02/23 10:22:08 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <vector>
# include <map>
# include <set>
# include <stdlib.h>

struct LocationConfig
{
	std::string					path;
	std::string					root;
	std::string					index;
	bool						autoindex;
	std::set<std::string>		methods;
	std::string					redirect;
	std::string					uploadStore;
	std::string					cgiExtension;
	std::string					cgiPath;

	LocationConfig() : autoindex(false)
	{
		methods.insert("GET");
	}
};

struct ServerConfig
{
	int								port;
	std::string						serverName;
	size_t							clientMaxBodySize;
	std::map<int, std::string>		errorPages;
	std::vector<LocationConfig>		locations;

	ServerConfig() : port(8080), serverName("localhost"), clientMaxBodySize(1048576) {}
};

struct Config
{
	size_t						bufferSize;
	size_t						numSockets;
	size_t						connectionsMax;
	std::vector<int>			socketPorts;
	std::vector<ServerConfig>	servers;
	Config();
};

#endif
