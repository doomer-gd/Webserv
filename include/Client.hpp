/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 12:28:41 by ikulik            #+#    #+#             */
/*   Updated: 2026/01/08 14:47:48 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "main.hpp"

enum	ClientState
{
	CS_RECIEVING,
	CS_SENDING,
	CS_PARSING,
	CS_EXEC_REQUEST,
	CS_NUM_STATES
};

class Client
{
	private:
		int				socket_fd;
		ClientState		e_currentState;
		IState			currentState;
	public:
		Client();
		Client(int socket_fd);
		Client(const Client& other);
		Client&	operator=(const Client& other);
		~Client();
		int			GetSocketFd( void ) const;
		ClientState	GetEnumState( void ) const;
		IState		GetCurrentState ( void ) const;

		int	UpdateState( void );
		int	Execute
};

// expr = term { + term }
// term = factor { + factor }
// factor = NUM | '(' expr ')'

#endif
