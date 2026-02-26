/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wrappers.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 14:35:16 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/26 20:50:28 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRAPPERS_HPP
# define WRAPPERS_HPP

#include "../main/main.hpp"

enum EventType
{
	ETYPE_CLIENT,
	ETYPE_SOCKET,
	ETYPE_TIMER,
	ETYPE_NUM_TYPES
};

//timers can be added to the epoll registry
struct EpollConent
{
	public:
		const EventType	type;

		EpollConent(EventType type): type(type) {};
		virtual ~EpollConent(){};
};

#endif
