/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sender.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:50:47 by ikulik            #+#    #+#             */
/*   Updated: 2026/01/09 17:51:12 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SENDER_HPP
# define SENDER_HPP
# include "../main/main.hpp"

class Sender: public IState
{
	private:
		std::string&	buffer;
	public:
		Sender(std::string& buffer);
		~Sender();

		void		Initialize();
		int			Execute();
		ClientState	Exit();
};

#endif
