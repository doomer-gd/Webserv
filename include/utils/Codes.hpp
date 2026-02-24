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

#endif
