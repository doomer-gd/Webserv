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
# include <sys/types.h>
# include "../main/Config.hpp"
# include "../utils/StateMachine.hpp"
# include "../services/HttpMessage.hpp"
# include "../utils/EpollWrappers.hpp"

class AConnection;

class Client : public EpollConent
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
		pid_t					cgiPid;

		HttpRequest				request;

		void	CleanUpStates(void);
	public:
		Client();
		Client(AConnection* connection, const ConfigMain& config,
			const ServerConfig* serverConfig);
		~Client();

		int				GetFd( void ) const;
		ClientState		GetEnumState( void ) const;
		IState*			GetCurrentState ( void ) const;
		AConnection*	GetConnection( void ) const;
		void			SetState(ClientState state);
		void			InnitializeStates(const ConfigMain& config);
		ClientState		UpdateState(void);
		HttpRequest&	GetRequest();
		std::string&	GetBuffer();
		time_t			GetLastActivity(void) const;
		int				GetCgiReadFd(void) const;
		int				GetCgiWriteFd(void) const;
		void			SetupCgi(int writeFd, int readFd, pid_t pid,
							const std::string* body);
		void			CloseCgiReadFd(void);
		void			CloseCgiWriteFd(void);
		int				DriveCgiRead(void);
		int				DriveCgiWrite(void);
		void			TransitionToSending(void);
};

#endif
