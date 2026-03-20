/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtrofyme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 00:00:00 by vtrofyme          #+#    #+#             */
/*   Updated: 2026/02/22 00:00:00 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "services/RequestHandler.hpp"
#include "services/CgiHandler.hpp"
#include "utils/MimeTypes.hpp"
#include "main/Webserv.hpp"
#include "utils/Basics.hpp"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

RequestHandler::RequestHandler(const ServerConfig& config) : serverConfig(config) {}

std::string	RequestHandler::getStatusText(int code) const
{
	switch (code)
	{
		case 200: return "OK";
		case 201: return "Created";
		case 204: return "No Content";
		case 301: return "Moved Permanently";
		case 400: return "Bad Request";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 413: return "Payload Too Large";
		case 500: return "Internal Server Error";
		default: return "Unknown";
	}
}

std::string	RequestHandler::getDefaultErrorPage(int code) const
{
	std::stringstream	ss;

	ss << "<html><head><title>" << code << " " << getStatusText(code)
		<< "</title></head><body><center><h1>" << code << " "
		<< getStatusText(code) << "</h1></center><hr>"
		<< "<center>webserv</center></body></html>";
	return ss.str();
}

HttpResponse	RequestHandler::makeErrorResponse(int code) const
{
	HttpResponse	resp;

	resp.statusCode = code;
	resp.statusText = getStatusText(code);

	std::map<int, std::string>::const_iterator it = serverConfig.errorPages.find(code);
	if (it != serverConfig.errorPages.end())
	{
		std::ifstream	file(it->second.c_str());
		if (file.is_open())
		{
			std::stringstream	ss;
			ss << file.rdbuf();
			resp.body = ss.str();
			file.close();
		}
		else
			resp.body = getDefaultErrorPage(code);
	}
	else
		resp.body = getDefaultErrorPage(code);

	resp.headers["Content-Type"] = "text/html";
	resp.headers["Content-Length"] = toString(resp.body.size());
	resp.headers["Connection"] = "close";
	return resp;
}

HttpResponse	RequestHandler::makeRedirect(const std::string& url) const
{
	HttpResponse	resp;

	resp.statusCode = 301;
	resp.statusText = "Moved Permanently";
	resp.headers["Location"] = url;
	resp.headers["Content-Length"] = "0";
	resp.headers["Connection"] = "close";
	return resp;
}

bool	RequestHandler::isDirectory(const std::string& path) const
{
	struct stat	st;

	if (stat(path.c_str(), &st) != 0)
		return false;
	return S_ISDIR(st.st_mode);
}

bool	RequestHandler::fileExists(const std::string& path) const
{
	struct stat	st;

	return (stat(path.c_str(), &st) == 0);
}

static std::string	sanitizeUri(const std::string& uri)
{
	std::vector<std::string>	parts;
	std::istringstream			stream(uri);
	std::string					segment;

	while (std::getline(stream, segment, '/'))
	{
		if (segment.empty() || segment == ".")
			continue;
		if (segment == "..")
		{
			if (!parts.empty())
				parts.pop_back();
		}
		else
			parts.push_back(segment);
	}

	std::string	result;
	for (size_t i = 0; i < parts.size(); i++)
		result += "/" + parts[i];
	if (result.empty())
		result = "/";
	return result;
}

std::string	RequestHandler::resolvePath(const std::string& root,
	const std::string& uri, const std::string& locPath) const
{
	std::string	safeUri = sanitizeUri(uri);
	std::string	relativePath;

	if (safeUri.size() >= locPath.size())
		relativePath = safeUri.substr(locPath.size());

	if (!relativePath.empty() && relativePath[0] == '/')
		relativePath = relativePath.substr(1);

	std::string	fullPath = root;
	if (!fullPath.empty() && fullPath[fullPath.size() - 1] != '/')
		fullPath += "/";
	fullPath += relativePath;

	return fullPath;
}

const LocationConfig*	RequestHandler::findLocation(const std::string& uri) const
{
	const LocationConfig*	bestMatch = NULL;
	size_t					bestLen = 0;

	for (size_t i = 0; i < serverConfig.locations.size(); i++)
	{
		const std::string&	locPath = serverConfig.locations[i].path;

		if (uri.compare(0, locPath.size(), locPath) == 0)
		{
			if (locPath != "/" && locPath.size() < uri.size()
				&& uri[locPath.size()] != '/')
				continue;
			if (locPath.size() > bestLen)
			{
				bestLen = locPath.size();
				bestMatch = &serverConfig.locations[i];
			}
		}
	}
	return bestMatch;
}

HttpResponse	RequestHandler::handleRequest(const HttpRequest& req) const
{
	if (req.body.size() > (size_t)serverConfig.clientMaxBodySize)
		return makeErrorResponse(413);

	const LocationConfig*	loc = findLocation(req.uri);
	if (!loc)
		return makeErrorResponse(404);

	if (!loc->redirect.empty())
		return makeRedirect(loc->redirect);

	if (loc->methods.find(req.method) == loc->methods.end())
		return makeErrorResponse(405);

	if (!loc->cgiExtension.empty() && !loc->cgiPath.empty())
	{
		std::string	ext = loc->cgiExtension;
		if (req.uri.size() >= ext.size()
			&& req.uri.compare(req.uri.size() - ext.size(), ext.size(), ext) == 0)
		{
			CgiHandler	cgi;
			return cgi.executeCgi(req, *loc, serverConfig);
		}
	}

	if (req.method == "GET")
		return handleGet(req, *loc);
	else if (req.method == "POST")
		return handlePost(req, *loc);
	else if (req.method == "DELETE")
		return handleDelete(req, *loc);

	return makeErrorResponse(405);
}

HttpResponse	RequestHandler::handleGet(const HttpRequest& req,
	const LocationConfig& loc) const
{
	std::string	filePath = resolvePath(loc.root, req.uri, loc.path);

	if (isDirectory(filePath))
	{
		if (!filePath.empty() && filePath[filePath.size() - 1] != '/')
			filePath += "/";

		if (!loc.index.empty())
		{
			std::string	indexPath = filePath + loc.index;
			if (fileExists(indexPath))
				return serveFile(indexPath);
		}

		if (loc.autoindex)
			return serveDirectoryListing(filePath, req.uri);

		return makeErrorResponse(403);
	}

	if (!fileExists(filePath))
		return makeErrorResponse(404);

	return serveFile(filePath);
}

HttpResponse	RequestHandler::serveFile(const std::string& filePath) const
{
	std::ifstream	file(filePath.c_str(), std::ios::binary);

	if (!file.is_open())
		return makeErrorResponse(403);

	std::stringstream	ss;
	ss << file.rdbuf();
	file.close();

	HttpResponse	resp;

	resp.statusCode = 200;
	resp.statusText = "OK";
	resp.body = ss.str();
	resp.headers["Content-Type"] = getMimeType(filePath);
	resp.headers["Content-Length"] = toString(resp.body.size());
	resp.headers["Connection"] = "close";
	return resp;
}

HttpResponse	RequestHandler::serveDirectoryListing(const std::string& dirPath,
	const std::string& uri) const
{
	DIR*	dir = opendir(dirPath.c_str());

	if (!dir)
		return makeErrorResponse(403);

	std::stringstream	html;

	html << "<html><head><title>Index of " << uri << "</title></head><body>";
	html << "<h1>Index of " << uri << "</h1><hr><pre>";

	struct dirent*	entry;

	while ((entry = readdir(dir)) != NULL)
	{
		std::string	name = entry->d_name;
		if (name == ".")
			continue;

		std::string	href = uri;
		if (!href.empty() && href[href.size() - 1] != '/')
			href += "/";
		href += name;

		std::string	fullPath = dirPath + name;
		if (isDirectory(fullPath))
			name += "/";

		html << "<a href=\"" << href << "\">" << name << "</a>\n";
	}

	closedir(dir);

	html << "</pre><hr></body></html>";

	HttpResponse	resp;

	resp.statusCode = 200;
	resp.statusText = "OK";
	resp.body = html.str();
	resp.headers["Content-Type"] = "text/html";
	resp.headers["Content-Length"] = toString(resp.body.size());
	resp.headers["Connection"] = "close";
	return resp;
}

static std::string	extractMultipartBody(const HttpRequest& req)
{
	std::map<std::string, std::string>::const_iterator ct
		= req.headers.find("content-type");
	if (ct == req.headers.end())
		return req.body;

	size_t	bpos = ct->second.find("boundary=");
	if (bpos == std::string::npos)
		return req.body;

	std::string	boundary = "--" + ct->second.substr(bpos + 9);
	size_t	start = req.body.find(boundary);
	if (start == std::string::npos)
		return req.body;

	start = req.body.find("\r\n\r\n", start);
	if (start == std::string::npos)
		return req.body;
	start += 4;

	size_t	end = req.body.find(boundary, start);
	if (end == std::string::npos)
		return req.body;

	if (end >= 2 && req.body[end - 2] == '\r' && req.body[end - 1] == '\n')
		end -= 2;

	return req.body.substr(start, end - start);
}

static std::string	extractMultipartFilename(const HttpRequest& req)
{
	std::map<std::string, std::string>::const_iterator ct
		= req.headers.find("content-type");
	if (ct == req.headers.end())
		return "";

	size_t	bpos = ct->second.find("boundary=");
	if (bpos == std::string::npos)
		return "";

	std::string	boundary = "--" + ct->second.substr(bpos + 9);
	size_t	start = req.body.find(boundary);
	if (start == std::string::npos)
		return "";

	size_t	headerEnd = req.body.find("\r\n\r\n", start);
	if (headerEnd == std::string::npos)
		return "";

	std::string	headers = req.body.substr(start, headerEnd - start);
	size_t	fnamePos = headers.find("filename=\"");
	if (fnamePos == std::string::npos)
		return "";

	fnamePos += 10;
	size_t	fnameEnd = headers.find("\"", fnamePos);
	if (fnameEnd == std::string::npos)
		return "";

	return headers.substr(fnamePos, fnameEnd - fnamePos);
}

HttpResponse	RequestHandler::handlePost(const HttpRequest& req,
	const LocationConfig& loc) const
{
	if (loc.uploadStore.empty())
		return makeErrorResponse(403);

	std::string	fileName;
	std::string	fileContent;
	bool		isMultipart = false;

	std::map<std::string, std::string>::const_iterator ct
		= req.headers.find("content-type");
	if (ct != req.headers.end()
		&& ct->second.find("multipart/form-data") != std::string::npos)
	{
		isMultipart = true;
		fileName = extractMultipartFilename(req);
		fileContent = extractMultipartBody(req);
	}

	if (fileName.empty())
	{
		size_t	lastSlash = req.uri.rfind('/');
		if (lastSlash != std::string::npos && lastSlash + 1 < req.uri.size())
			fileName = req.uri.substr(lastSlash + 1);
		else
			fileName = "upload";
	}

	if (!isMultipart)
		fileContent = req.body;

	std::string	filePath = loc.uploadStore;
	if (!filePath.empty() && filePath[filePath.size() - 1] != '/')
		filePath += "/";
	filePath += fileName;

	std::ofstream	file(filePath.c_str(), std::ios::binary);

	if (!file.is_open())
		return makeErrorResponse(500);

	file.write(fileContent.c_str(), fileContent.size());
	file.close();

	HttpResponse	resp;

	resp.statusCode = 201;
	resp.statusText = "Created";
	resp.body = "<html><body><h1>File Uploaded Successfully</h1></body></html>";
	resp.headers["Content-Type"] = "text/html";
	resp.headers["Content-Length"] = toString(resp.body.size());
	resp.headers["Connection"] = "close";
	return resp;
}

HttpResponse	RequestHandler::handleDelete(const HttpRequest& req,
	const LocationConfig& loc) const
{
	std::string	filePath = resolvePath(loc.root, req.uri, loc.path);

	if (!fileExists(filePath))
		return makeErrorResponse(404);

	if (isDirectory(filePath))
		return makeErrorResponse(403);

	if (std::remove(filePath.c_str()) != 0)
		return makeErrorResponse(500);

	HttpResponse	resp;

	resp.statusCode = 200;
	resp.statusText = "OK";
	resp.body = "<html><body><h1>File Deleted Successfully</h1></body></html>";
	resp.headers["Content-Type"] = "text/html";
	resp.headers["Content-Length"] = toString(resp.body.size());
	resp.headers["Connection"] = "close";
	return resp;
}
