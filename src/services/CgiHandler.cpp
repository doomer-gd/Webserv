/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtrofyme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 20:06:21 by vtrofyme          #+#    #+#             */
/*   Updated: 2026/02/23 20:06:21 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "CgiHandler.hpp"
#include "Basics.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstring>
#include <sstream>

std::vector<std::string>	CgiHandler::buildEnv(const HttpRequest& req,
	const LocationConfig& loc, const ServerConfig& srv,
	const std::string& scriptPath) const
{
	std::vector<std::string>	env;

	env.push_back("REQUEST_METHOD=" + req.method);
	env.push_back("QUERY_STRING=" + req.queryString);
	env.push_back("CONTENT_LENGTH=" + toString(req.body.size()));
	env.push_back("SCRIPT_FILENAME=" + scriptPath);
	env.push_back("SCRIPT_NAME=" + loc.path);
	env.push_back("PATH_INFO=" + req.uri);
	env.push_back("SERVER_NAME=" + srv.serverName);
	env.push_back("SERVER_PORT=" + toString(srv.port));
	env.push_back("SERVER_PROTOCOL=" + req.httpVersion);
	env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	env.push_back("REDIRECT_STATUS=200");

	if (req.headers.count("content-type"))
		env.push_back("CONTENT_TYPE=" + req.headers.find("content-type")->second);

	for (std::map<std::string, std::string>::const_iterator it = req.headers.begin();
		it != req.headers.end(); ++it)
	{
		std::string	key = "HTTP_";
		for (size_t i = 0; i < it->first.size(); i++)
		{
			if (it->first[i] == '-')
				key += '_';
			else
				key += (char)toupper(it->first[i]);
		}
		key += "=" + it->second;
		env.push_back(key);
	}

	return env;
}

char**	CgiHandler::vecToCharArray(const std::vector<std::string>& vec) const
{
	char**	arr = new char*[vec.size() + 1];

	for (size_t i = 0; i < vec.size(); i++)
	{
		arr[i] = new char[vec[i].size() + 1];
		std::strcpy(arr[i], vec[i].c_str());
	}
	arr[vec.size()] = NULL;
	return arr;
}

void	CgiHandler::freeCharArray(char** arr, size_t size) const
{
	for (size_t i = 0; i < size; i++)
		delete[] arr[i];
	delete[] arr;
}

HttpResponse	CgiHandler::parseCgiOutput(const std::string& output) const
{
	HttpResponse	resp;

	resp.statusCode = 200;
	resp.statusText = "OK";

	size_t	headerEnd = output.find("\r\n\r\n");
	if (headerEnd == std::string::npos)
		headerEnd = output.find("\n\n");

	if (headerEnd == std::string::npos)
	{
		resp.body = output;
		resp.headers["Content-Type"] = "text/html";
	}
	else
	{
		std::string			headerSection = output.substr(0, headerEnd);
		size_t				bodyStart = headerEnd + 2;

		if (output[headerEnd] == '\r')
			bodyStart = headerEnd + 4;

		resp.body = output.substr(bodyStart);

		std::istringstream	stream(headerSection);
		std::string			line;

		while (std::getline(stream, line))
		{
			if (!line.empty() && line[line.size() - 1] == '\r')
				line = line.substr(0, line.size() - 1);

			size_t	colonPos = line.find(':');
			if (colonPos == std::string::npos)
				continue;

			std::string	key = line.substr(0, colonPos);
			std::string	value = line.substr(colonPos + 1);

			size_t	valStart = value.find_first_not_of(" \t");
			if (valStart != std::string::npos)
				value = value.substr(valStart);

			if (key == "Status")
			{
				std::istringstream	ss(value);
				ss >> resp.statusCode;
				size_t	spacePos = value.find(' ');
				if (spacePos != std::string::npos)
					resp.statusText = value.substr(spacePos + 1);
			}
			else
				resp.headers[key] = value;
		}
	}

	resp.headers["Content-Length"] = toString(resp.body.size());
	resp.headers["Connection"] = "close";
	return resp;
}

HttpResponse	CgiHandler::executeCgi(const HttpRequest& req,
	const LocationConfig& loc, const ServerConfig& srv) const
{
	std::string	scriptPath = loc.root;
	if (!scriptPath.empty() && scriptPath[scriptPath.size() - 1] != '/')
		scriptPath += "/";

	std::string	relativePath;
	if (req.uri.size() >= loc.path.size())
		relativePath = req.uri.substr(loc.path.size());
	if (!relativePath.empty() && relativePath[0] == '/')
		relativePath = relativePath.substr(1);
	scriptPath += relativePath;

	char	absPathBuf[4096];
	if (realpath(scriptPath.c_str(), absPathBuf) != NULL)
		scriptPath = absPathBuf;

	int	pipeIn[2];
	int	pipeOut[2];

	if (pipe(pipeIn) == -1)
	{
		HttpResponse	resp;
		resp.statusCode = 500;
		resp.statusText = "Internal Server Error";
		resp.body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
		resp.headers["Content-Type"] = "text/html";
		resp.headers["Content-Length"] = toString(resp.body.size());
		return resp;
	}
	if (pipe(pipeOut) == -1)
	{
		close(pipeIn[0]);
		close(pipeIn[1]);
		HttpResponse	resp;
		resp.statusCode = 500;
		resp.statusText = "Internal Server Error";
		resp.body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
		resp.headers["Content-Type"] = "text/html";
		resp.headers["Content-Length"] = toString(resp.body.size());
		return resp;
	}

	pid_t	pid = fork();

	if (pid == -1)
	{
		close(pipeIn[0]);
		close(pipeIn[1]);
		close(pipeOut[0]);
		close(pipeOut[1]);
		HttpResponse	resp;
		resp.statusCode = 500;
		resp.statusText = "Internal Server Error";
		resp.body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
		resp.headers["Content-Type"] = "text/html";
		resp.headers["Content-Length"] = toString(resp.body.size());
		return resp;
	}

	if (pid == 0)
	{
		close(pipeIn[1]);
		close(pipeOut[0]);
		dup2(pipeIn[0], STDIN_FILENO);
		dup2(pipeOut[1], STDOUT_FILENO);
		close(pipeIn[0]);
		close(pipeOut[1]);

		std::string	scriptDir = scriptPath;
		size_t		lastSlash = scriptDir.rfind('/');
		if (lastSlash != std::string::npos)
			scriptDir = scriptDir.substr(0, lastSlash);
		chdir(scriptDir.c_str());

		std::vector<std::string>	envVec = buildEnv(req, loc, srv, scriptPath);
		char**						envp = vecToCharArray(envVec);

		char*	argv[3];
		argv[0] = const_cast<char*>(loc.cgiPath.c_str());
		argv[1] = const_cast<char*>(scriptPath.c_str());
		argv[2] = NULL;

		execve(loc.cgiPath.c_str(), argv, envp);
		freeCharArray(envp, envVec.size());
		_exit(1);
	}

	close(pipeIn[0]);
	close(pipeOut[1]);

	if (!req.body.empty())
		write(pipeIn[1], req.body.c_str(), req.body.size());
	close(pipeIn[1]);

	std::string	output;
	char		buf[4096];
	ssize_t		bytesRead;

	while ((bytesRead = read(pipeOut[0], buf, sizeof(buf) - 1)) > 0)
	{
		buf[bytesRead] = '\0';
		output += buf;
	}
	close(pipeOut[0]);

	int	status;
	waitpid(pid, &status, 0);

	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		HttpResponse	resp;
		resp.statusCode = 500;
		resp.statusText = "Internal Server Error";
		resp.body = "<html><body><h1>500 CGI Error</h1></body></html>";
		resp.headers["Content-Type"] = "text/html";
		resp.headers["Content-Length"] = toString(resp.body.size());
		resp.headers["Connection"] = "close";
		return resp;
	}

	return parseCgiOutput(output);
}
