/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpMessage.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtrofyme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 10:23:04 by vtrofyme          #+#    #+#             */
/*   Updated: 2026/02/23 10:23:04 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMESSAGE_HPP
# define HTTPMESSAGE_HPP

# include <string>
# include <map>
# include <sstream> //sstream

struct HttpRequest
{
	std::string							method;
	std::string							uri;
	std::string							queryString;
	std::string							httpVersion;
	std::map<std::string, std::string>	headers;
	std::string							body;

	void	clear()
	{
		method.clear();
		uri.clear();
		queryString.clear();
		httpVersion.clear();
		headers.clear();
		body.clear();
	}
};

struct HttpResponse
{
	int									statusCode;
	std::string							statusText;
	std::map<std::string, std::string>	headers;
	std::string							body;

	HttpResponse() : statusCode(200), statusText("OK") {}

	std::string	toString() const
	{
		std::stringstream	ss;

		ss << "HTTP/1.1 " << statusCode << " " << statusText << "\r\n";
		for (std::map<std::string, std::string>::const_iterator it = headers.begin();
			it != headers.end(); ++it)
		{
			ss << it->first << ": " << it->second << "\r\n";
		}
		ss << "\r\n";
		ss << body;
		return ss.str();
	}
};

#endif
