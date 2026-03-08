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


#include "../../include/services/Executer.hpp"
#include "../../include/services/Client.hpp"
#include "../../include/main/Webserv.hpp"
#include "../../include/services/HttpMessage.hpp"
#include "../../include/services/RequestHandler.hpp"

Executer::Executer(std::string& buffer, Client* client, const ServerConfig* config)
	: buffer(buffer), client(client), serverConfig(config) {}

Executer::~Executer() {}

void	Executer::Initialize()
{
	(void)client;
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
	HttpResponse	resp = handler.handleRequest(client->GetRequest());

	buffer = resp.toString();
	return FINISHED;
}

int	Executer::Exit()
{
	return CS_SENDING;
}
