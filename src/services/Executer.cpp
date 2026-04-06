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

#include "main.hpp"
#include "HttpMessage.hpp"
#include "RequestHandler.hpp"
#include "CgiHandler.hpp"

Executer::Executer(std::string& buffer, Client* client, const ServerConfig* config)
	: buffer(buffer), client(client), serverConfig(config), cgiStarted(false) {}

Executer::~Executer() {}

void	Executer::Initialize()
{
	cgiStarted = false;
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

		client->SetupCgi(proc.pipeFd, proc.pid);
		cgiStarted = true;
		return FINISHED;
	}

	HttpResponse	resp = handler.handleRequest(req);
	buffer = resp.toString();
	return FINISHED;
}

ClientState	Executer::Exit()
{
	if (cgiStarted)
		return CS_CGI_READING;
	return CS_SENDING;
}
