/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtrofyme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 20:05:55 by vtrofyme          #+#    #+#             */
/*   Updated: 2026/02/23 20:05:55 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include <string>
# include <vector>
# include "../main/Config.hpp"
# include "../services/HttpMessage.hpp"

class CgiHandler
{
	private:
		std::vector<std::string>	buildEnv(const HttpRequest& req,
										const LocationConfig& loc,
										const ServerConfig& srv,
										const std::string& scriptPath) const;
		char**						vecToCharArray(
										const std::vector<std::string>& vec) const;
		void						freeCharArray(char** arr, size_t size) const;
		HttpResponse				parseCgiOutput(const std::string& output) const;

	public:
		HttpResponse	executeCgi(const HttpRequest& req,
							const LocationConfig& loc,
							const ServerConfig& srv) const;
};

#endif
