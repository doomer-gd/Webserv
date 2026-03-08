/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollWrappers.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 14:35:16 by ikulik            #+#    #+#             */
/*   Updated: 2026/03/08 11:30:20 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLL_WRAPPERS_HPP
# define EPOLL_WRAPPERS_HPP

enum EventType
{
	ETYPE_CLIENT,
	ETYPE_SOCKET,
	ETYPE_TIMER,
	ETYPE_NUM_TYPES
};

//timers can be added to the epoll registry
class EpollConent
{
	public:
		EventType	type;

		EpollConent(EventType type): type(type) {};
		virtual ~EpollConent(){};
};

#endif
