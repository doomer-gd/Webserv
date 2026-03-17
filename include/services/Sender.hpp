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
# include "../services/ClientStateMachine.hpp"

class Sender: public IState
{
	private:
		std::string&	buffer;
		int				fd;
		size_t			bytesSent;
	public:
		Sender(std::string& buffer, int fd);
		~Sender();

		void		Initialize();
		int			Execute();
		ClientState	Exit();
};

#endif
