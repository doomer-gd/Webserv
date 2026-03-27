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
# include "Connection.hpp"

typedef std::set< std::pair<time_t, Client*> > TimeoutQueue;
typedef std::map<Client*, TimeoutQueue::iterator> MapClientTimeout;

//Idea for timeouts: we save a set(it is sorted by time) to see which is the closest timeout.
//If the closest is up, remove all timed-out clients.
//Also save a map from clients to the set
//to be able to remove them arbitrarilty if, for example, its job is done

class TaskManager
{
	private:
		std::vector<Socket>		socks;
		std::set<Client*>		clients;
		TimeoutQueue			queueTimeout;
		MapClientTimeout		mapTimeout;
		Poller					poller;
		std::queue<Client*>		queueExec;
		const ConfigMain		config;
		bool					isOn;

		int		AddClient(int fd, Socket& sock);
		int		OpenSockets();
		int		HandleClientUpdate(Client* client);
		int		HandleInitResult(bool hasSuccess, int failureCode);
		void	CheckTimeouts(void);
		void	CleanupAllClients(void);
		void	RemoveClient(Client* client);
		int		AddToTimeoutQueue(Client* client, time_t delay);
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
