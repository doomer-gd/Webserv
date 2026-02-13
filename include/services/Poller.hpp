/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 14:16:02 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/13 16:08:18 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLLER_HPP
# define POLLER_HPP
# include "main.hpp"

typedef struct epoll_event e_event_t;
class Poller
{
	private:
		int						fdRegistry;
		int						connectionsMax;
		int						connectionsCurrent;
		int						numNewEvents;
		std::vector<e_event_t>	events;
	public:
		Poller();
		Poller(const Config& config);
		~Poller();

		e_event_t&	GetEvent(int index);

		int			CreatePoll(void);
		int			AddFd(int fd, int mask, void* data);
		int			RemoveFd(int fd);
		int			AddConnection(AConnection* connection, int mask);
		int			RemoveConnection(AConnection* connection);
		int			AddClient(Client* client, int mask);
		int			RemoveClient(Client* client);
		int			SetFdFlags(int fd, int mask, void* data);
		int			Poll (void);
};

#endif
