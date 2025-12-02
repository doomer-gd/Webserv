/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 15:17:46 by ikulik            #+#    #+#             */
/*   Updated: 2025/12/02 17:08:58 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

int Webserv::exitCode_ = 0;

int	Webserv::Exit(ExitCode errorCode)
{
	exitCode_ = errorCode;
	switch (errorCode)
	{
	case E_FAILURE:
		return (EXIT_FAILURE);

	case E_WRONG_ARGUMENTS:
		std::cerr << "webserv: wrong number of arguments" << std::endl;
		return (EXIT_FAILURE);

	default:
		return (EXIT_SUCCESS);
	}
	return (errorCode);
}
