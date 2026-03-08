/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigTokenizer.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 17:05:39 by ikulik            #+#    #+#             */
/*   Updated: 2026/03/08 11:18:08 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_TOKENIZER_HPP
# define CONFIG_TOKENIZER_HPP
# include <iostream>
# include <string>
# include <fstream>
# include <map>
# include "ConfigDefines.hpp"
# include "../utils/StateMachine.hpp"

//Stplits istream into tokens according to nginx config rules
class ConfigTokenizer
{
	public:
		typedef int (ConfigTokenizer::*StateHandler)(std::istream&, std::string&);
	private:
		static std::map<EPasreState, StateHandler>	dictStates;
		EPasreState		stateCurrent;
		StateHandler	functionCurrent;
		bool			isTokenEnded;
		void	SetUpParseFunctions(void);
		int		advanceRegular(std::istream& source, std::string& buffer);
		int		advanceDobleQ(std::istream& source, std::string& buffer);
		int		advanceSingleQ(std::istream& source, std::string& buffer);
		int		advanceComment(std::istream& source, std::string& buffer);
		void	CheckBrakingChar(std::istream& source, std::string& buffer, char ch);
		int		WaitForChar(std::istream& source, std::string& buffer, char expected);
		void	SetDefaultState(std::string& buffer);
	public:
		ConfigTokenizer();
		StateStatus	GetNextToken(std::istream& source, std::string& buffer);
};

#endif