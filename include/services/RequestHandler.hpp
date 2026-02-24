/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtrofyme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 10:23:13 by vtrofyme          #+#    #+#             */
/*   Updated: 2026/02/23 10:23:13 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include <string>
# include "../main/Config.hpp"
# include "../services/HttpMessage.hpp"

class RequestHandler
{
	private:
		const ServerConfig&	serverConfig;

		const LocationConfig*	findLocation(const std::string& uri) const;
		HttpResponse			handleGet(const HttpRequest& req,
									const LocationConfig& loc) const;
		HttpResponse			handlePost(const HttpRequest& req,
									const LocationConfig& loc) const;
		HttpResponse			handleDelete(const HttpRequest& req,
									const LocationConfig& loc) const;
		HttpResponse			serveFile(const std::string& filePath) const;
		HttpResponse			serveDirectoryListing(const std::string& dirPath,
									const std::string& uri) const;
		HttpResponse			makeErrorResponse(int code) const;
		HttpResponse			makeRedirect(const std::string& url) const;
		std::string				getDefaultErrorPage(int code) const;
		std::string				getStatusText(int code) const;
		std::string				resolvePath(const std::string& root,
									const std::string& uri,
									const std::string& locPath) const;
		bool					isDirectory(const std::string& path) const;
		bool					fileExists(const std::string& path) const;

	public:
		RequestHandler(const ServerConfig& config);

		HttpResponse	handleRequest(const HttpRequest& req) const;
};

#endif
