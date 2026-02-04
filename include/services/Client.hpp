/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 12:28:41 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/04 17:53:38 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "main.hpp"

enum	ClientState
{
	CS_READING_HEADER,
	CS_READING_BODY,
	CS_EXEC_REQUEST,
	CS_SENDING,
	CS_NUM_STATES
};

class Client
{
	private:
		AConnection*	connection;
		std::string		buffer;
		size_t			bufferSize;
		bool			isReady;
		ClientState		e_currentState;
		Command			command;
		IState*			currentState;
		IState*			states[CS_NUM_STATES];

		void		SetUpStates(Config& config);
	public:
		Client();
		Client(Connection* connection, Config& config);
		Client(const Client& other);
		Client&	operator=(const Client& other);
		~Client();

		int			GetSocketFd( void ) const;
		ClientState	GetEnumState( void ) const;
		IState*		GetCurrentState ( void ) const;

		int		UpdateState(void);
};

// expr = term { + term }
// term = factor { + factor }
// factor = NUM | '(' expr ')'

#endif
