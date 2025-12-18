/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 14:44:12 by ikulik            #+#    #+#             */
/*   Updated: 2025/12/18 18:05:39 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

int	main(int argc, char** argv)
{
	Socket	sock;

	if (argc != 2)
		return (Webserv::Exit(E_WRONG_ARGUMENTS));
	std::cout << "Welcome to the webserver! " << argv[1] << std::endl;
	sock.OpenMainSocket(8080);
	return (Webserv::Exit(E_SUCCESS));
}
