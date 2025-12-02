/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 14:44:12 by ikulik            #+#    #+#             */
/*   Updated: 2025/12/02 16:55:02 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

int	main(int argc, char** argv)
{
	if (argc != 2)
		return (Webserv::Exit(E_WRONG_ARGUMENTS));
	std::cout << "Welcome to the webserver! " << argv[1] << std::endl;

	return (Webserv::Exit(E_SUCCESS));
}
