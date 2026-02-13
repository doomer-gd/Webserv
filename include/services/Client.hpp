/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 12:28:41 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/13 17:00:40 by ikulik           ###   ########.fr       */
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
	CS_DEAD,
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

		void	SetUpStates(const Config& config);
		void	CleanUpStates(void);
	public:
		Client();
		Client(AConnection* connection, const Config& config);
		Client(const Client& other);
		Client&	operator=(const Client& other);
		~Client();

		int				GetSocketFd( void ) const;
		ClientState		GetEnumState( void ) const;
		IState*			GetCurrentState ( void ) const;
		AConnection*	GetConnection( void ) const;

		ClientState		UpdateState(void);
};

// expr = term { + term }
// term = factor { + factor }
// factor = NUM | '(' expr ')'

#endif
