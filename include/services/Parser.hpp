/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 14:59:57 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/22 00:00:00 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <string>
# include <sstream>
# include <unistd.h>
# include "../main/Config.hpp"
# include "../utils/StateMachine.hpp"
# include "../services/HttpMessage.hpp"

class Client;

class Parser: public IState
{
	private:
		std::string&	bufferMain;
		std::string		bufferTemp;
		size_t			bufferSize;
		ssize_t			bytesRead;
		int				fd;
		char*			readBuffer;
		Client*			client;
		HttpRequest*	request;
		bool			headersDone;
		size_t			contentLength;
		bool			isChunked;

		int		ParseRequestLine(const std::string& line);
		int		ParseHeaderLine(const std::string& line);
		int		ParseHeaders();
		int		ParseChunkedBody();
		int		CheckBodyComplete();
	public:
		Parser(std::string& buffer, const Config& config, Client* client, int fd);
		virtual ~Parser();

		void	Initialize();
		int		Execute();
		int		Exit();

		void	LinkRequest(HttpRequest* req);
};

#endif
