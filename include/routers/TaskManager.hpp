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
		Poller					poller;
		std::queue<Client*>		queueExec;
		const ConfigMain			config;
		bool					isOn;

		int	AddClient(int fd, Socket& sock);
		int	OpenSockets();
		int	HandleClientUpdate(Client* client);
		int	HandleInitResult(bool hasSuccess, int failureCode);
	public:
		TaskManager();
		TaskManager(const ConfigMain& config);

		int	InnitializeServer();
		int	StartMainLoop();
		int	OpenNewConnections(Socket* sock);
		int	RunPolledEvents();
		int	ExecuteCommands();
};

#endif
