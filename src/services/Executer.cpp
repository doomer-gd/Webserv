/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 16:22:01 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/22 00:00:00 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main/main.hpp"
#include "services/HttpMessage.hpp"
#include "services/RequestHandler.hpp"
#include "services/CgiHandler.hpp"

Executer::Executer(std::string& buffer, Client* client)
	: buffer(buffer), client(client), serverConfig(NULL),
	  cgiStarted(false), cgiHasBody(false) {}

Executer::~Executer() {}

void	Executer::Initialize()
{
	cgiStarted = false;
	cgiHasBody = false;
	client->SelectServerConfig();
	serverConfig = client->GetServerConfig();
	Webserv::Log("Initializing executer");
}

int	Executer::Execute()
{
	Webserv::Log("Executing request: " + client->GetRequest().method
		+ " " + client->GetRequest().uri);

	if (!serverConfig)
	{
		buffer = "HTTP/1.1 500 Internal Server Error\r\n"
			"Content-Type: text/html\r\nContent-Length: 51\r\n"
			"Connection: close\r\n\r\n"
			"<html><body><h1>500 Server Error</h1></body></html>";
		return FINISHED;
	}

	RequestHandler	handler(*serverConfig);
	const HttpRequest& req = client->GetRequest();

	const LocationConfig*	matched = handler.getLocation(req.uri);
	int						bodyLimit = serverConfig->clientMaxBodySize;
	if (matched && matched->clientMaxBodySize >= 0)
		bodyLimit = matched->clientMaxBodySize;

	if (req.body.size() > (size_t)bodyLimit)
	{
		buffer = "HTTP/1.1 413 Payload Too Large\r\n"
			"Content-Type: text/html\r\nContent-Length: 56\r\n"
			"Connection: close\r\n\r\n"
			"<html><body><h1>413 Payload Too Large</h1></body></html>";
		return FINISHED;
	}

	if (handler.isCgiRequest(req))
	{
		const LocationConfig* loc = handler.getLocation(req.uri);
		if (!loc)
		{
			buffer = "HTTP/1.1 404 Not Found\r\n"
				"Content-Type: text/html\r\nContent-Length: 46\r\n"
				"Connection: close\r\n\r\n"
				"<html><body><h1>404 Not Found</h1></body></html>";
			return FINISHED;
		}
		if (loc->methods.find(req.method) == loc->methods.end())
		{
			HttpResponse resp = handler.handleRequest(req);
			buffer = resp.toString();
			return FINISHED;
		}

		CgiHandler	cgi;
		CgiProcess	proc = cgi.startCgi(req, *loc, *serverConfig);
		if (proc.pid < 0)
		{
			buffer = "HTTP/1.1 500 Internal Server Error\r\n"
				"Content-Type: text/html\r\nContent-Length: 51\r\n"
				"Connection: close\r\n\r\n"
				"<html><body><h1>500 Server Error</h1></body></html>";
			return FINISHED;
		}

		cgiHasBody = !req.body.empty();
		client->SetupCgi(proc.writeFd, proc.readFd, proc.pid,
			cgiHasBody ? &client->GetRequest().body : NULL);
		if (!cgiHasBody)
			client->CloseCgiWriteFd();
		cgiStarted = true;
		return FINISHED;
	}

	HttpResponse	resp = handler.handleRequest(req);
	buffer = resp.toString();
	return FINISHED;
}

int	Executer::Exit()
{
	if (cgiStarted)
	{
		if (cgiHasBody)
			return CS_CGI_WRITING;
		return CS_CGI_READING;
	}
	return CS_SENDING;
}
