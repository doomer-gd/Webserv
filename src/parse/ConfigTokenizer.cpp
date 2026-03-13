/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigTokenizer.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 19:08:44 by ikulik            #+#    #+#             */
/*   Updated: 2026/03/13 14:43:04 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parse/ConfigTokenizer.hpp"
#include "../../include/parse/ConfigDefines.hpp"

ConfigTokenizer::ConfigTokenizer(): stateCurrent(EPS_REGULAR), isTokenEnded(false)
{
	SetUpParseFunctions();
}

std::map<EPasreState, ConfigTokenizer::StateHandler>	ConfigTokenizer::dictStates;

void	ConfigTokenizer::SetUpParseFunctions(void)
{
	if (dictStates.empty() == false)
		return ;
	dictStates[EPS_REGULAR] = &ConfigTokenizer::advanceRegular;
	dictStates[EPS_DOUBLE_QUOTE] = &ConfigTokenizer::advanceDobleQ;
	dictStates[EPS_SINGLE_QUOTE] = &ConfigTokenizer::advanceSingleQ;
	dictStates[EPS_COMMENT] = &ConfigTokenizer::advanceComment;
}

StateStatus	ConfigTokenizer::GetNextToken(std::istream& source, std::string& buffer)
{
	StateStatus	status;

	SetDefaultState(buffer);
	if (source.fail())
		return ERROR;
	if (source.peek() == EOF)
		return FINISHED;
	FlushWhitespace(source);
	while (source.peek() != EOF)
	{
		status = (StateStatus)(this->*functionCurrent)(source, buffer);
		if (status == ERROR)
			return ERROR;
		if (isTokenEnded)
			return status;
		functionCurrent = dictStates[stateCurrent];
		if (functionCurrent == NULL)
			return ERROR;
	}
	return FINISHED;
}

void	ConfigTokenizer::SetDefaultState(std::string& buffer)
{
	buffer.clear();
	isTokenEnded = false;
	stateCurrent = EPS_REGULAR;
	functionCurrent = &ConfigTokenizer::advanceRegular;
}

void	ConfigTokenizer::FlushWhitespace(std::istream& source)
{
	if (source.fail())
		return ;
	while (source.peek() != EOF)
	{
		if (std::isspace(source.get()) == 0)
		{
			source.unget();
			return ;
		}
	}
}

int	ConfigTokenizer::advanceRegular(std::istream& source, std::string& buffer)
{
	char	ch;

	while (source.peek() != EOF)
	{
		ch = source.get();
		if (std::isspace(ch) != 0)
		{
			isTokenEnded = true;
			return EXECUTING;
		}
		switch (ch)
		{
			case '\'':
				stateCurrent = EPS_SINGLE_QUOTE;
				return EXECUTING;
			case '\"':
				stateCurrent = EPS_DOUBLE_QUOTE;
				return EXECUTING;
			case '#':
				stateCurrent = EPS_COMMENT;
				return EXECUTING;
			case ';':
			case '{':
			case '}':
				CheckBrakingChar(source, buffer, ch);
				return EXECUTING;
			default:
				buffer.push_back(ch);
		}
	}
	return FINISHED;
}

void	ConfigTokenizer::CheckBrakingChar(std::istream& source, std::string& buffer, char ch)
{
	if (buffer.empty())
		buffer.push_back(ch);
	else
		source.unget();
	isTokenEnded = true;
}

int	ConfigTokenizer::advanceDobleQ(std::istream& source, std::string& buffer)
{
	return WaitForChar(source, buffer, '\"');
}

int	ConfigTokenizer::advanceSingleQ(std::istream& source, std::string& buffer)
{
	return WaitForChar(source, buffer, '\'');
}

int	ConfigTokenizer::advanceComment(std::istream& source, std::string& buffer)
{
	(void)buffer;
	isTokenEnded = true;
	while (source.peek() != EOF)
	{
		if (source.get() == '\n')
			return EXECUTING;
	}
	return FINISHED;
}

int	ConfigTokenizer::WaitForChar(std::istream& source, std::string& buffer, char expected)
{
	char	ch;

	while (source.peek() != EOF)
	{
		ch = source.get();
		if (ch == expected)
		{
			stateCurrent = EPS_REGULAR;
			return EXECUTING;
		}
		buffer.push_back(ch);
	}
	return ERROR;
}
