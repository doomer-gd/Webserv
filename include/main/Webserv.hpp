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

# include "../main/Config.hpp"
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

class	Webserv
{
	private:
		static std::ostream&	logStream;
		Config					config;

		int			ReadConfig();
		static void	DisplayTimestamp(void);
	public:
		static int	exitCode_;

		Webserv();
		~Webserv();
		static int	Exit(int errorCode);
		static void	Log(const std::string& message);
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
