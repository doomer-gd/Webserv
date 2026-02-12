/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:28:00 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/12 17:23:11 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_HPP
# define MAIN_HPP


# define DEF_MAX_CONNS 10
# define DEF_BUFFER_SIZE 1024
# define DEF_NUM_SOCKETS 1

//default C libraries
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/epoll.h>
# include <signal.h>
# include <dirent.h>
# include <fcntl.h>
# include <sys/types.h>
# include <errno.h>
//default C++ libraries
# include <iostream>
# include <vector>
# include <queue>
# include <unordered_set>

//custom headers
# include "./utils/Codes.hpp"
# include "./utils/Basics.h"
# include "./utils/Webserv.hpp"

# include "./services/Client.hpp"
# include "./services/ClientStateMachine.hpp"
# include "./services/Poller.hpp"

# include "./routers/Connection.hpp"
# include "./routers/Socket.hpp"
# include "./routers/TaskManager.hpp"

# include "./services/Parser.hpp"
# include "./services/Reader.hpp"
# include "./services/Executer.hpp"
# include "./services/Sender.hpp"
# include "./services/Poller.hpp"

#endif
