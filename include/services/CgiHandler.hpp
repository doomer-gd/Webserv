/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtrofyme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 20:05:55 by vtrofyme          #+#    #+#             */
/*   Updated: 2026/02/23 20:05:55 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include <string>
# include <vector>
# include <sys/types.h>
# include "../main/Config.hpp"
# include "../services/HttpMessage.hpp"
# include "../utils/StateMachine.hpp"

struct CgiProcess
{
	int		readFd;
	int		writeFd;
	pid_t	pid;
};

class CgiHandler
{
	private:
		std::vector<std::string>	buildEnv(const HttpRequest& req,
										const LocationConfig& loc,
										const ServerConfig& srv,
										const std::string& scriptPath) const;
		char**						vecToCharArray(
										const std::vector<std::string>& vec) const;
		void						freeCharArray(char** arr, size_t size) const;

	public:
		HttpResponse	parseCgiOutput(const std::string& output) const;
		CgiProcess		startCgi(const HttpRequest& req,
							const LocationConfig& loc,
							const ServerConfig& srv) const;
};

class CgiInWriter: public IState
{
	private:
		int					writeFd;
		const std::string*	body;
		size_t				bytesSent;
	public:
		CgiInWriter();
		~CgiInWriter();

		void		Setup(int fd, const std::string* bodyPtr);
		int			GetWriteFd(void) const;
		void		ClosePipe(void);
		void		Initialize();
		int			Execute();
		int			Exit();
};

class CgiState: public IState
{
	private:
		std::string&	buffer;
		int				pipeFd;
		std::string		output;
	public:
		CgiState(std::string& buffer);
		~CgiState();

		void		Setup(int fd);
		int			GetPipeFd(void) const;
		void		ClosePipe(void);
		void		Initialize();
		int			Execute();
		int			Exit();
};

#endif
