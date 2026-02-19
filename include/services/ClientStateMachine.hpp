/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientStateMachine.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 13:32:12 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/04 14:41:03 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_STATE_MACHINE_HPP
# define CLIENT_STATE_MACHINE_HPP

enum StateStatus
{
	EXECUTING,
	FINISHED,
	ERROR,
	NUM_STATUS
};

class	IState
{
	public:
		virtual void		Initialize() = 0;
		virtual int			Execute() = 0;
		virtual ClientState	Exit() = 0;
		virtual ~IState() = 0;
};

#endif
