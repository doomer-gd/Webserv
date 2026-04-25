/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:21:58 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/22 00:00:00 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "services/Parser.hpp"
#include "services/HttpMessage.hpp"
#include <vector>
#include <sys/epoll.h>
#include <errno.h>

Parser::Parser(std::string& buffer, const ConfigMain& config, Client* client, int fd):
	bufferMain(buffer), bufferSize(config.bufferSize), bytesRead(0),
	fd(fd), readBuffer(NULL), client(client), request(NULL),
	headersDone(false), contentLength(0), isChunked(false)
{
	bufferTemp.reserve(bufferSize);
	readBuffer = new char[bufferSize];
}

Parser::~Parser()
{
	delete[] readBuffer;
}

void	Parser::Initialize()
{
	if (request)
		request->clear();
	(void)bytesRead; //compiler complained of unused variable
	(void)client; //here too
	headersDone = false;
	contentLength = 0;
	isChunked = false;
	bufferMain.clear();
}

int	Parser::Execute()
{
	ssize_t n = read(fd, readBuffer, bufferSize);
	if (n > 0)
		bufferMain.append(readBuffer, n);
	else if (n == 0)
		return ERROR;
	else if (bufferMain.empty())
		return EXECUTING;

	if (!headersDone)
	{
		int result = ParseHeaders();
		if (result == ERROR)
			return ERROR;
		if (!headersDone)
			return EXECUTING;
	}

	if (headersDone)
	{
		if (isChunked)
			return ParseChunkedBody();
		return CheckBodyComplete();
	}

	return EXECUTING;
}

int	Parser::Exit()
{
	return CS_EXEC_REQUEST;
}

void	Parser::LinkRequest(HttpRequest* req)
{
	request = req;
}

static std::string	trimString(const std::string& str)
{
	size_t	start = str.find_first_not_of(" \t");
	size_t	end = str.find_last_not_of(" \t\r\n");

	if (start == std::string::npos)
		return "";
	return str.substr(start, end - start + 1);
}

int	Parser::ParseRequestLine(const std::string& line)
{
	std::istringstream	iss(line);

	if (!(iss >> request->method >> request->uri >> request->httpVersion))
		return ERROR;

	size_t	queryPos = request->uri.find('?');
	if (queryPos != std::string::npos)
	{
		request->queryString = request->uri.substr(queryPos + 1);
		request->uri = request->uri.substr(0, queryPos);
	}

	if (request->method != "GET" && request->method != "POST"
		&& request->method != "DELETE")
		return ERROR;

	if (request->httpVersion != "HTTP/1.0" && request->httpVersion != "HTTP/1.1")
		return ERROR;

	return FINISHED;
}

int	Parser::ParseHeaderLine(const std::string& line)
{
	size_t	colonPos = line.find(':');

	if (colonPos == std::string::npos)
		return ERROR;

	std::string	key = trimString(line.substr(0, colonPos));
	std::string	value = trimString(line.substr(colonPos + 1));

	for (size_t i = 0; i < key.size(); i++)
		key[i] = tolower(key[i]);

	request->headers[key] = value;
	return FINISHED;
}

int	Parser::ParseHeaders()
{
	size_t	headerEnd = bufferMain.find("\r\n\r\n");

	if (headerEnd == std::string::npos)
		return EXECUTING;

	std::string			headerSection = bufferMain.substr(0, headerEnd);
	std::string			remaining = bufferMain.substr(headerEnd + 4);
	std::istringstream	stream(headerSection);
	std::string			line;

	if (!std::getline(stream, line))
		return ERROR;

	line = trimString(line);
	if (ParseRequestLine(line) == ERROR)
		return ERROR;

	while (std::getline(stream, line))
	{
		line = trimString(line);
		if (line.empty())
			continue;
		if (ParseHeaderLine(line) == ERROR)
			return ERROR;
	}

	headersDone = true;

	if (request->headers.count("content-length"))
	{
		std::istringstream	clStream(request->headers["content-length"]);
		clStream >> contentLength;
	}

	if (request->headers.count("transfer-encoding"))
	{
		std::string	te = request->headers["transfer-encoding"];
		for (size_t i = 0; i < te.size(); i++)
			te[i] = tolower(te[i]);
		if (te.find("chunked") != std::string::npos)
			isChunked = true;
	}

	bufferMain = remaining;
	return FINISHED;
}

int	Parser::CheckBodyComplete()
{
	if (request->method == "GET" || request->method == "DELETE")
	{
		contentLength = 0;
		return FINISHED;
	}

	if (contentLength == 0)
		return FINISHED;

	if (bufferMain.size() >= contentLength)
	{
		request->body = bufferMain.substr(0, contentLength);
		bufferMain = bufferMain.substr(contentLength);
		return FINISHED;
	}
	return EXECUTING;
}

int	Parser::ParseChunkedBody()
{
	while (true)
	{
		size_t	lineEnd = bufferMain.find("\r\n");

		if (lineEnd == std::string::npos)
			return EXECUTING;

		std::string		sizeStr = bufferMain.substr(0, lineEnd);
		unsigned long	chunkSize = 0;
		std::istringstream	hexStream(sizeStr);

		hexStream >> std::hex >> chunkSize;

		if (chunkSize == 0)
		{
			bufferMain = bufferMain.substr(lineEnd + 2);
			if (bufferMain.size() >= 2 && bufferMain[0] == '\r' && bufferMain[1] == '\n')
				bufferMain = bufferMain.substr(2);
			return FINISHED;
		}

		size_t	dataStart = lineEnd + 2;

		if (bufferMain.size() < dataStart + chunkSize + 2)
			return EXECUTING;

		request->body.append(bufferMain, dataStart, chunkSize);
		bufferMain = bufferMain.substr(dataStart + chunkSize + 2);
	}
}
