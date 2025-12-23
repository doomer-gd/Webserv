/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 14:16:02 by ikulik            #+#    #+#             */
/*   Updated: 2025/12/23 15:08:33 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLLER_HPP
# define POLLER_HPP
# include "main.hpp"

class Poller
{
	private:
		int					epollFd;
		int					status;
		struct epoll_event	events[DEF_MAX_CONNS];
		Socket*				baseSocket;
	public:
		Poller();
		Poller(Socket* baseSocket);
		~Poller();

		void		CreatePoll(void);
		void		AddFd(int fd, int mask);
		void		RemoveFd(int fd);
		void		SetFdFlags(int fd, int mask);
		int			Poll (void);
		uint32_t	GetEventStatus(int index);
};



#endif
