/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:32:28 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/22 00:00:00 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READER_HPP
# define READER_HPP

# include <string>
# include "../utils/StateMachine.hpp"

class Reader: public IState
{
	private:
		std::string&	buffer;
	public:
		Reader(std::string& buffer);
		~Reader();

		void	Initialize();
		int		Execute();
		int		Exit();
};

#endif
