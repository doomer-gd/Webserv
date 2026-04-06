/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:44:57 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/22 00:00:00 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTER_HPP
# define EXECUTER_HPP

# include <string>
# include "../main/Config.hpp"
# include "../utils/StateMachine.hpp"

class Client;

class Executer: public IState
{
	private:
		std::string&		buffer;
		Client*				client;
		const ServerConfig*	serverConfig;
		bool				cgiStarted;
	public:
		Executer(std::string& buffer, Client* client, const ServerConfig* config);
		~Executer();

		void	Initialize();
		int		Execute();
		int		Exit();
};

#endif
