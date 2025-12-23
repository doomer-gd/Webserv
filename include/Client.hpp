/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 12:28:41 by ikulik            #+#    #+#             */
/*   Updated: 2025/12/23 17:51:12 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "main.hpp"

enum	ClientState
{
	RECIEVING,
	SENDING,
	PARSING,
	EXEC_REQUEST,
	NUM_STATES
};

class Client
{
	private:
		int				socket_fd;
		ClientState		e_currentState;
	public:
		Client();
		Client(int socket_fd);
		Client(const Client& other);
		Client&	operator=(const Client& other);
		~Client();
};

// expr = term { + term }
// term = factor { + factor }
// factor = NUM | '(' expr ')'


#endif
