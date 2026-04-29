/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Closer.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtrofyme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 10:00:00 by vtrofyme          #+#    #+#             */
/*   Updated: 2026/04/29 10:00:00 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLOSER_HPP
# define CLOSER_HPP

# include "../utils/StateMachine.hpp"

class Closer: public IState
{
	private:
		int		fd;
		bool	shutdownDone;
	public:
		Closer(int fd);
		~Closer();

		void	Initialize();
		int		Execute();
		int		Exit();
};

#endif
