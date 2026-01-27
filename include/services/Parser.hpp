/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 14:59:57 by ikulik            #+#    #+#             */
/*   Updated: 2026/01/27 16:05:47 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include "main.hpp"

class Parser: public IState
{
	private:
		std::string&	bufferMain;
		std::string		bufferTemp;
		size_t			bufferSize;
		ssize_t			bytesRead;
		int				somethingToReturn;
		Client*			client;
	public:
		Parser(std::string& buffer, Config& config, Client* client);

		void		Initialize();
		int			Execute();
		ClientState	Exit();

		int	GetRequest() {return 0;};
};

#endif
