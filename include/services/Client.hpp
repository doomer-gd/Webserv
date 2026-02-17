/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 12:28:41 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/17 14:56:09 by ikulik           ###   ########.fr       */
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
		AConnection*			connection;
		std::string				buffer;
		size_t					bufferSize;
		bool					isReady;
		ClientState				e_currentState;
		Command					command;
		IState*					currentState;
		std::vector<IState*>	states;

			void	CleanUpStates(void);
	public:
		Client();
		Client(AConnection* connection, const Config& config);
		Client(AConnection* connection, const Config& config, std::vector<IState*>& states);
		~Client();

		int				GetFd( void ) const;
		ClientState		GetEnumState( void ) const;
		IState*			GetCurrentState ( void ) const;
		AConnection*	GetConnection( void ) const;
		void			SetState(ClientState state);
		void			InnitializeStates(const Config& config);
		ClientState		UpdateState(void);
};

// expr = term { + term }
// term = factor { + factor }
// factor = NUM | '(' expr ')'

#endif
