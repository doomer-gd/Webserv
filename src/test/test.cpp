/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 17:39:27 by ikulik            #+#    #+#             */
/*   Updated: 2025/12/18 18:42:13 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"
#include "stdio.h"

int	main(int argc, char** argv)
{
	Socket	sock;
	char	buffer[1000];

	if (argc != 2)
		return (1);
	sock.OpenMainSocket(atoi(argv[1]));
	while (42)
	{
		int	newConn;
		socklen_t	socklen;

		newConn = accept(sock.GetMainSocketFd(), NULL, &socklen);
		if (newConn != -1)
		{
			read(newConn, buffer, 999);
			printf("%s\n", buffer);
			close(newConn);
			sock.CloseMainSocket();
			break ;
		}
		usleep(1000);
	}
	return (EXIT_SUCCESS);
}
