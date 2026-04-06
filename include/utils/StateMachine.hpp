/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientStateMachine.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 13:32:12 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/22 00:00:00 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_STATE_MACHINE_HPP
# define CLIENT_STATE_MACHINE_HPP

enum ClientState
{
	CS_READING_HEADER,
	CS_READING_BODY,
	CS_EXEC_REQUEST,
	CS_CGI_READING,
	CS_SENDING,
	CS_DEAD,
	CS_NUM_STATES
};


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
		virtual void	Initialize() = 0;
		virtual int		Execute() = 0;
		virtual int		Exit() = 0;
		virtual ~IState();
};

#endif
