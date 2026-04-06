/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 15:18:43 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/22 00:00:00 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "../../include/main/Config.hpp"
# include "../../include/routers/TaskManager.hpp"
# include "../../include/parse/ConfigParser.hpp"
# include <string>
# include <iostream>
# include <ostream>
# include <iomanip>

//General workflow:
//accept new connections and allocate memory (if available) for a new Client entity
//run epoll to see what can be read/written
//do read of size bufferSize and parse what was read
//execute requests that are complete
//write what can be written to sockets
//if client has no more requests, close socket, clean up
//repeat from start

//New vision: control everything with epoll:
//both sockets and client fds go into epoll.
//When it returns something, is it's a socket we open new connections
//if it's a client we execute its state.
//It can also be a timerfd type thing but that may not be neccessary
//if we can use a set to track timeouts

class	Webserv
{
	private:
		static std::ostream&	logStream;
		ConfigParser*			confParser;
		ConfigMain*				config;
		TaskManager*			managerMain;

		static void	DisplayTimestamp(void);
	public:
		static int	exitCode_;

		Webserv();
		~Webserv();
		static int		Exit(int errorCode);
		static void		Log(const std::string& message);
		int				Innitialize(const char* fileNameConf);
		ConfigMain*		GetConfig(void) const;
		TaskManager*	GetTaskManager(void) const;

		class Except : public std::exception
		{
			private:
				const char*	errorStr;
			public:
				Except(const char* errorStr):errorStr(errorStr){};
				Except(const std::string& errorStr):errorStr(errorStr.c_str()){};
				const char*	what() const throw() { return errorStr; }
		};
};

#endif
