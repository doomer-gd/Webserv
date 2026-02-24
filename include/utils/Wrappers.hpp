/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wrappers.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 14:35:16 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/24 15:18:09 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRAPPERS_HPP
# define WRAPPERS_HPP

#include "../main/main.hpp"

enum EventType
{
	ETYPE_CLIENT,
	ETYPE_SOCKET,
	ETYPE_NUM_TYPES
};

struct EpollConent
{
	public:
		EventType	type;

		EpollConent(EventType type): type(type) {};
		virtual ~EpollConent(){};
};

#endif
