/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientStateMachine.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 13:32:12 by ikulik            #+#    #+#             */
/*   Updated: 2025/12/23 17:56:51 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_STATE_MACHINE_HPP
# define CLIENT_STATE_MACHINE_HPP

class IState
{
	void	Initialize();
	int		Execute();
	IState	Exit();
};



#endif
