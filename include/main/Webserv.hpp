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
# include <fstream>
# include <iomanip>

//General workflow:
//accept new connections and allocate memory (if available) for a new Client entity
//run epoll to see what can be read/written
//do read of size bufferSize and parse what was read
//execute requests that are complete
//write what can be written to sockets
//if client has no more requests, close socket, clean up
//repeat from start


class	Webserv
{
	private:
		static std::ostream&	logStream;
		static std::fstream		logFile;
		static void	DisplayTimestamp(std::ostream& stream);
	public:
		static const char*		configPath;
		static ConfigMain*		config;
		static TaskManager*		managerMain;
		
		Webserv();
		~Webserv();
		static int		Exit(int errorCode);
		static void		Log(const std::string& message);
		static int		OpenLogFile(const char* logFile);
		static void		HandleSignals( void );
		static int		CheckArguments(int argc, char** argv);
		static int		ReadConfig( void );
		static int		StartServer( void );

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
