/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 16:19:47 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/18 17:12:22 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TASKMANAGER_HPP
# define TASKMANAGER_HPP
# include "main.hpp"

class TaskManager
{
	private:
		std::vector<Socket>			socks;
		std::set<Client*>	clients;
		Poller						poller;
		std::queue<Client*>			queueExec;
		const Config				config;
		bool						isOn;

		int	AddClient(int fd, Socket& sock);
		int	HandleClientUpdate(Client* client);
		int	HandleInitResult(bool hasSuccess, int failureCode);
	public:
		TaskManager();
		TaskManager(const Config& config);

		int	InnitializeServer();
		int	StartMainLoop();
		int	OpenNewConnections();
		int	RunPolledEvents();
		int	ExecuteCommands();
};

#endif

