/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sender.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:50:47 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/22 00:00:00 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SENDER_HPP
# define SENDER_HPP

# include <string>
# include "../utils/StateMachine.hpp"

class Sender: public IState
{
	private:
		std::string&	buffer;
	public:
		Sender(std::string& buffer);
		~Sender();

		void	Initialize();
		int		Execute();
		int		Exit();
};

#endif
