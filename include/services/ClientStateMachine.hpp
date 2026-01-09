/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientStateMachine.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 13:32:12 by ikulik            #+#    #+#             */
/*   Updated: 2026/01/09 15:14:24 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_STATE_MACHINE_HPP
# define CLIENT_STATE_MACHINE_HPP

class	IState
{
	virtual void		Initialize() = 0;
	virtual int			Execute() = 0;
	virtual ClientState	Exit() = 0;
};

#endif
