/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 16:19:47 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/04 17:35:01 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASKMANAGER_HPP
# define TASKMANAGER_HPP
# include "main.hpp"

class TaskManager
{
	private:
		std::vector<Socket>			socks;
		std::unordered_set<Client*>	clients;
		Poller						poller;
		std::queue<Client*>			execQueue;
	public:
		int	OpenNewConnections();
		int	GetPolledEvents();
		int	ReadConnections();
		int	ExecuteCommands();
		int	WriteConnections();
};

#endif

