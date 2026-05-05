/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 14:44:12 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/13 18:07:23 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main/main.hpp"

#define CHECK_OK(expr) \
	do { int result = (expr); if ((result) != E_SUCCESS) return (result); } while(0)

extern volatile sig_atomic_t g_signal;

int	main(int argc, char** argv)
{
	CHECK_OK(Webserv::CheckArguments(argc, argv));
	Webserv::HandleSignals();
	CHECK_OK(Webserv::ReadConfig());
	CHECK_OK(Webserv::StartServer());
	return (Webserv::Exit(E_SUCCESS));
}
