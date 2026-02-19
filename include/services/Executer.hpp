/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:44:57 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/18 17:14:06 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTER_HPP
# define EXECUTER_HPP

# include "../main/main.hpp"

class Command
{
	public:
		void	DoSomething(void);
};

class Executer: public IState
{
	private:
		std::string&	buffer;
		Command&		command;
	public:
		Executer(std::string& buffer, Command& command);
		~Executer();

		void		Initialize();
		int			Execute();
		ClientState	Exit();
};



#endif
