/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 15:18:43 by ikulik            #+#    #+#             */
/*   Updated: 2026/01/09 16:13:59 by ikulik           ###   ########.fr       */
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
	size_t	bufferSize;
};

class	Webserv
{
	private:
		static int	exitCode_;
	public:
		static int	Exit(ExitCode errorCode);
		Webserv(){};
		~Webserv(){};
};

#endif
