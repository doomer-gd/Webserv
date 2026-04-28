/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:28:00 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/22 00:00:00 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_HPP
# define MAIN_HPP
# pragma once

//apparently good practice is to include each header separately
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
//default C++ libraries
# include <iostream>
# include <ostream>
# include <sstream>
# include <fstream>
# include <iomanip>
# include <vector>
# include <queue>
# include <set>
# include <map>
# include <string>

//custom headers — order matters!
# include "../main/Config.hpp"
# include "../main/Webserv.hpp"

# include "../parse/ConfigDefines.hpp"
# include "../parse/ConfigParser.hpp"
# include "../parse/ConfigSetters.hpp"
# include "../parse/ConfigTokenizer.hpp"

# include "../routers/Connection.hpp"
# include "../routers/Socket.hpp"
# include "../routers/TaskManager.hpp"

# include "../services/CgiHandler.hpp"
# include "../services/Client.hpp"
# include "../services/Executer.hpp"
# include "../services/HttpMessage.hpp"
# include "../services/Parser.hpp"
# include "../services/Poller.hpp"
# include "../services/RequestHandler.hpp"
# include "../services/Sender.hpp"

# include "../utils/Basics.hpp"
# include "../utils/Codes.hpp"
# include "../utils/EpollWrappers.hpp"
# include "../utils/Heirarchy.hpp"
# include "../utils/MimeTypes.hpp"
# include "../utils/StateMachine.hpp"

#endif
