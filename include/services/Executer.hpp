/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:44:57 by ikulik            #+#    #+#             */
/*   Updated: 2026/01/09 17:53:05 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTER_HPP
# define EXECUTER_HPP
# include "main.hpp"

class Command
{
	private:
		int	doSomething;
};

class Executer: public IState
{
	private:
		std::string&	buffer;
		Command*		command;
	public:
		Executer(std::string& buffer);

		void		Initialize();
		int			Execute();
		ClientState	Exit();
};



#endif
