/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Codes.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:21:20 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/22 00:00:00 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODES_HPP
# define CODES_HPP

# define DEF_MAX_CONNS 10
# define DEF_BUFFER_SIZE 1024
# define DEF_NUM_SOCKETS 1
# define DEF_PORT 80
# define DEF_SERV_NAME "localhost"
# define DEF_MAX_BODY_SIZE 1048576
# define DEF_TIMEOUT 10

enum ExitCode
{
	E_SUCCESS,
	E_FAILURE,
	E_WRONG_ARGUMENTS,
	E_SOCKET_CREATE,
	E_BIND_ERROR,
	E_SOCKET_FLAG,
	E_EPOLL_CREATE,
	NUM_ERRORS
};

enum ClientState
{
	CS_READING_HEADER,
	CS_READING_BODY,
	CS_EXEC_REQUEST,
	CS_SENDING,
	CS_DEAD,
	CS_NUM_STATES
};

#endif
