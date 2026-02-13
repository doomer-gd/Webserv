/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 15:18:43 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/13 17:43:19 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "main.hpp"

//General workflow:
//accept new connections and allocate memory (if available) for a new Client entity
//run epoll to see what can be read/written
//do read of size bufferSize and parse what was read
//execute requests that are complete
//write what can be written to sockets
//if client has no more requests, close socket, clean up
//repeat from start

struct Config
{
	size_t				bufferSize;
	size_t				numSockets;
	size_t				connectionsMax;
	std::vector<int>	socketPorts;
	Config();
};

class	Webserv
{
	private:
		static int				exitCode_;
		static std::ostream&	logStream;
		Config					config;

		int		ReadConfig();
		static void	DisplayTimestamp(void);
	public:
		Webserv();
		~Webserv();
		static int	Exit(ExitCode errorCode);
		static void	Log(const std::string& message);

		int	Initialize();
		int	Update();
		int	CleanUp();
};

#endif
