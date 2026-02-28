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

struct LocationConfig : public ConfigItem
{
	std::string				path; //location arguments, can be more than one, so probably should be a vector/set
	std::string				root; //root
	std::string				index; //index, also an array/set
	bool					autoindex; //autoindex
	std::set<std::string>	methods; // methods
	std::string				redirect; //return
	std::string				uploadStore;//upload_store
	std::string				cgiExtension; //location argument
	std::string				cgiPath; //include

	LocationConfig() : autoindex(false)
	{
		methods.insert("GET");
	}
};

struct ServerConfig : public ConfigItem
{
	int							port; //listen
	std::string					serverName;//server_name, should be an array/set
	std::vector<std::string>	serverNames;
	int							clientMaxBodySize;//client_max_body_size
	std::map<int, std::string>	errorPages; //error_pages
	std::vector<LocationConfig>	locations; //location
	std::vector<IpPort>			portsArray; //listen

	ServerConfig() : port(DEF_PORT), serverName(DEF_SERV_NAME), clientMaxBodySize(DEF_MAX_BODY_SIZE) {}
};

struct ConfigMain : public ConfigItem
{
	int							bufferSize; //client_header_buffer_size
	int							bodyBufferSize;
	int							numSockets; //listen, depreciated
	int							connectionsMax; //worker_connections
	int							fdsMax; //worker_rlimit_nofile for total max fds
	std::string					logFileName; //error_log
	std::vector<int>			socketPorts; //listen, depreciated
	ConfigTimouts				timeOut;
	std::vector<ServerConfig>	servers; //server
	ConfigMain();
};

struct ConfigTimouts
{
	int	header;
	int	body;
	int	keepAlive;
	int	send;
	int	general;
	ConfigTimouts();
};

struct ConfigItem
{
	public:
		ConfigItem(){};
		virtual ~ConfigItem(){};
};

#endif
