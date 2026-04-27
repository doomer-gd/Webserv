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


#include "services/CgiHandler.hpp"
#include "utils/Basics.hpp"
#include "main/Webserv.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <fcntl.h>

std::vector<std::string>	CgiHandler::buildEnv(const HttpRequest& req,
	const LocationConfig& loc, const ServerConfig& srv,
	const std::string& scriptPath) const
{
	std::vector<std::string>	env;
	(void)loc;

	env.push_back("REQUEST_METHOD=" + req.method);
	env.push_back("QUERY_STRING=" + req.queryString);
	env.push_back("CONTENT_LENGTH=" + toString(req.body.size()));
	env.push_back("SCRIPT_FILENAME=" + scriptPath);
	env.push_back("SCRIPT_NAME=");
	env.push_back("PATH_INFO=" + req.uri);
	env.push_back("REQUEST_URI=" + req.uri);
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

CgiProcess	CgiHandler::startCgi(const HttpRequest& req,
	const LocationConfig& loc, const ServerConfig& srv) const
{
	CgiProcess	result;
	result.readFd = -1;
	result.writeFd = -1;
	result.pid = -1;

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

	std::string	cgiBin = loc.cgiPath;
	char	cgiAbsBuf[4096];
	if (realpath(cgiBin.c_str(), cgiAbsBuf) != NULL)
		cgiBin = cgiAbsBuf;

	int	pipeIn[2];
	int	pipeOut[2];

	if (pipe(pipeIn) == -1)
		return result;
	if (pipe(pipeOut) == -1)
	{
		close(pipeIn[0]);
		close(pipeIn[1]);
		return result;
	}

	pid_t	pid = fork();

	if (pid == -1)
	{
		close(pipeIn[0]);
		close(pipeIn[1]);
		close(pipeOut[0]);
		close(pipeOut[1]);
		return result;
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
		argv[0] = const_cast<char*>(cgiBin.c_str());
		argv[1] = const_cast<char*>(scriptPath.c_str());
		argv[2] = NULL;

		execve(cgiBin.c_str(), argv, envp);
		freeCharArray(envp, envVec.size());
		_exit(1);
	}

	close(pipeIn[0]);
	close(pipeOut[1]);

	fcntl(pipeIn[1], F_SETFL, O_NONBLOCK);
	fcntl(pipeOut[0], F_SETFL, O_NONBLOCK);

	result.writeFd = pipeIn[1];
	result.readFd = pipeOut[0];
	result.pid = pid;
	return result;
}

/* ========== CgiInWriter ========== */

CgiInWriter::CgiInWriter(): writeFd(-1), body(NULL), bytesSent(0) {}

CgiInWriter::~CgiInWriter()
{
	ClosePipe();
}

void	CgiInWriter::Setup(int fd, const std::string* bodyPtr)
{
	writeFd = fd;
	body = bodyPtr;
	bytesSent = 0;
}

int	CgiInWriter::GetWriteFd(void) const
{
	return writeFd;
}

void	CgiInWriter::ClosePipe(void)
{
	if (writeFd >= 0)
	{
		close(writeFd);
		writeFd = -1;
	}
}

void	CgiInWriter::Initialize()
{
	bytesSent = 0;
}

int	CgiInWriter::Execute()
{
	if (body == NULL || bytesSent >= body->size())
		return FINISHED;
	ssize_t n = write(writeFd, body->c_str() + bytesSent,
		body->size() - bytesSent);
	if (n > 0)
		bytesSent += n;
	else if (n < 0)
		return FINISHED;
	if (bytesSent >= body->size())
		return FINISHED;
	return EXECUTING;
}

int	CgiInWriter::Exit()
{
	return CS_CGI_READING;
}

/* ========== CgiState ========== */

CgiState::CgiState(std::string& buffer): buffer(buffer), pipeFd(-1) {}

CgiState::~CgiState()
{
	ClosePipe();
}

void	CgiState::Setup(int fd)
{
	pipeFd = fd;
	output.clear();
}

int	CgiState::GetPipeFd(void) const
{
	return pipeFd;
}

void	CgiState::ClosePipe(void)
{
	if (pipeFd >= 0)
	{
		close(pipeFd);
		pipeFd = -1;
	}
}

void	CgiState::Initialize()
{
	output.clear();
}

int	CgiState::Execute()
{
	char	buf[4096];
	ssize_t	n = read(pipeFd, buf, sizeof(buf));

	if (n > 0)
	{
		output.append(buf, n);
		return EXECUTING;
	}
	if (n == 0)
	{
		CgiHandler	handler;
		HttpResponse resp = handler.parseCgiOutput(output);
		buffer = resp.toString();
		return FINISHED;
	}
	return EXECUTING;
}

int	CgiState::Exit()
{
	return CS_SENDING;
}
