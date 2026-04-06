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
# include "../services/ClientStateMachine.hpp"

struct CgiProcess
{
	int		pipeFd;
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

class CgiState: public IState
{
	private:
		std::string&	buffer;
		int				pipeFd;
		pid_t			pid;
		std::string		output;
	public:
		CgiState(std::string& buffer);
		~CgiState();

		void		Setup(int fd, pid_t childPid);
		int			GetPipeFd(void) const;
		void		ClosePipe(void);
		void		Initialize();
		int			Execute();
		ClientState	Exit();
};

#endif
