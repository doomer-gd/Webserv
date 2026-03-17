/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 12:28:41 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/22 00:00:00 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <vector>
# include <ctime>
# include "../main/Config.hpp"
# include "../services/ClientStateMachine.hpp"
# include "../services/HttpMessage.hpp"

class AConnection;

class Client
{
	private:
		std::vector<IState*>	states;
		IState*					currentState;
		std::string				buffer;
		AConnection*			connection;
		const ServerConfig*		serverConfig;
		size_t					bufferSize;
		ClientState				e_currentState;
		bool					isReady;
		time_t					lastActivity;

		HttpRequest				request;

		void	CleanUpStates(void);
	public:
		Client();
		Client(AConnection* connection, const Config& config,
			const ServerConfig* serverConfig);
		~Client();

		int				GetFd( void ) const;
		ClientState		GetEnumState( void ) const;
		IState*			GetCurrentState ( void ) const;
		AConnection*	GetConnection( void ) const;
		void			SetState(ClientState state);
		void			InnitializeStates(const Config& config);
		ClientState		UpdateState(void);
		HttpRequest&	GetRequest();
		std::string&	GetBuffer();
		time_t			GetLastActivity(void) const;
};

#endif
