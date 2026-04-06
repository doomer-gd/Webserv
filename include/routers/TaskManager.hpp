/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 16:19:47 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/22 00:00:00 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASKMANAGER_HPP
# define TASKMANAGER_HPP

# include <vector>
# include <set>
# include <queue>
# include "../main/Config.hpp"
# include "../routers/Socket.hpp"
# include "../services/Client.hpp"
# include "../services/Poller.hpp"

class TaskManager
{
	private:
		std::vector<Socket>		socks;
		std::set<Client*>		clients;
		std::set<void*>			listenPtrs;
		Poller					poller;
		std::queue<Client*>		queueExec;
		const Config			config;
		bool					isOn;

		int		AddClient(int fd, Socket& sock);
		int		HandleClientUpdate(Client* client);
		int		HandleInitResult(bool hasSuccess, int failureCode);
		void	OpenNewConnections(Socket* sock);
		void	CheckTimeouts(void);
		void	CleanupAllClients(void);
	public:
		TaskManager();
		TaskManager(const Config& config);

		int	InnitializeServer();
		int	StartMainLoop();
		int	RunPolledEvents();
		int	ExecuteCommands();
};

#endif
