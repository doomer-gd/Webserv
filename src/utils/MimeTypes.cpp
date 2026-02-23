/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtrofyme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 00:00:00 by vtrofyme          #+#    #+#             */
/*   Updated: 2026/02/22 00:00:00 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MimeTypes.hpp"
#include <map>

static std::map<std::string, std::string>	initMimeTypes()
{
	std::map<std::string, std::string>	m;

	m[".html"] = "text/html";
	m[".htm"] = "text/html";
	m[".css"] = "text/css";
	m[".js"] = "application/javascript";
	m[".json"] = "application/json";
	m[".xml"] = "application/xml";
	m[".txt"] = "text/plain";
	m[".png"] = "image/png";
	m[".jpg"] = "image/jpeg";
	m[".jpeg"] = "image/jpeg";
	m[".gif"] = "image/gif";
	m[".svg"] = "image/svg+xml";
	m[".ico"] = "image/x-icon";
	m[".pdf"] = "application/pdf";
	m[".zip"] = "application/zip";
	m[".tar"] = "application/x-tar";
	m[".gz"] = "application/gzip";
	m[".mp3"] = "audio/mpeg";
	m[".mp4"] = "video/mp4";
	m[".webm"] = "video/webm";
	m[".woff"] = "font/woff";
	m[".woff2"] = "font/woff2";
	return m;
}

std::string	getMimeType(const std::string& path)
{
	static std::map<std::string, std::string>	mimeTypes = initMimeTypes();

	size_t	dotPos = path.rfind('.');
	if (dotPos == std::string::npos)
		return "application/octet-stream";

	std::string	ext = path.substr(dotPos);
	for (size_t i = 0; i < ext.size(); i++)
		ext[i] = tolower(ext[i]);

	std::map<std::string, std::string>::iterator it = mimeTypes.find(ext);
	if (it != mimeTypes.end())
		return it->second;
	return "application/octet-stream";
}
