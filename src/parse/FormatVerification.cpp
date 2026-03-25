/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FormatVerification.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 17:56:46 by ikulik            #+#    #+#             */
/*   Updated: 2026/03/25 17:42:11 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse/ConfigSetters.hpp"
#include "utils/Codes.hpp"
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <iostream> //debug
#include <netinet/in.h>

int	ConfigSetters::CheckSize(char literal, ESizeType mode)
{
	const struct Size*	formatArray;
	int					numVars = 0;

	switch (mode)
	{
	case SIZETYPE_BYTES:
		formatArray = g_memory_formats;
		numVars = g_memory_formats_size;
		break;
	case SIZETYPE_TIME:
		formatArray = g_time_formats;
		numVars = g_time_formats_size;
		break;
	default:
		return VER_ERROR;
		break;
	}
	for (int i = 0; i < numVars; i++)
	{
		if (literal == formatArray[i].param)
			return formatArray[i].size;
	}
	return VER_ERROR;
}

bool	ConfigSetters::IsUriChar(char ch)
{
	if (isalnum(ch) != 0)
		return true;
	return (strchr(CONF_URI_CHARS, ch) != NULL);
}

bool	ConfigSetters::VerifyNumber(const std::string& str)
{
	std::string::const_iterator	iter = str.begin();
	for (;iter != str.end(); iter++)
	{
		if (isdigit(*iter) == 0)
			return false;
	}
	return true;
}

bool	ConfigSetters::VerifySize(const std::string& str)
{
	std::string::const_iterator	iter = str.begin();
	int	result;

	while (iter != str.end() && (isdigit(*iter) != 0))
		iter++;
	if (iter == str.end())
		return true;
	if (iter == str.begin())
		return false;
	result = CheckSize(*iter, SIZETYPE_BYTES);
	if (result == VER_ERROR)
		return false;
	iter++;
	return iter == str.end();
}

bool	ConfigSetters::VerifyTime(const std::string& str)
{
	std::string::const_iterator	iter = str.begin();
	int	result;

	while (iter != str.end() && (isdigit(*iter) != 0))
		iter++;
	if (iter == str.end())
		return true;
	if (iter == str.begin())
		return false;
	result = CheckSize(*iter, SIZETYPE_TIME);
	if (result == VER_ERROR)
		return false;
	iter++;
	return iter == str.end();
}

bool	ConfigSetters::VerifyDirectory(const std::string& str)
{
	if (str[0] != '/')
		return false;
	return true;
}


bool	ConfigSetters::VerifyFilePath(const std::string& str)
{
	(void)str;
	return true;
}

bool	ConfigSetters::VerifyURL(const std::string& str)
{
	std::string::const_iterator	iter = str.begin();

	for (; iter != str.end(); iter++)
	{
		if (IsUriChar(*iter) == false)
			return false;
	}
	return true;
}

bool	ConfigSetters::VerifyMethod(const std::string& str)
{
	for (int i = 0; i< NUM_SUP_METHODS; i++)
	{
		if (g_supported_methods[i] == str)
			return true;
	}
	return false;
}

bool	ConfigSetters::VerifyIP(const std::string& str)
{
	std::istringstream	parser(str);
	int		nextNum;
	char	point;

	if (VerifyNumber(str))
		return true;
	for (int i = 0; i < 4; i++)
	{
		if (parser.peek() == EOF)
			return false;
		parser >> nextNum;
		if (nextNum < 0 || nextNum > 255)
			return false;
		parser >> point;
		if (i < 3 && point != '.')
			return false;
	}
	if (parser.peek() == EOF)
		return true;
	else if (point == ':')
	{
		std::string content;
		std::getline(parser, content);
		std::cout << "content: " << content << std::endl;
		return VerifyNumber(content);
	}
	return false;
}
bool	ConfigSetters::VerifyExtension(const std::string& str)
{
	std::string::const_iterator	iter = str.begin();

	if (str.size() < 2 || str[0] != '.')
		return false;
	iter++;
	for (; iter != str.end(); iter++)
	{
		if (isalnum(*iter) != 0)
			return false;
	}
	return true;
}

int	ConfigSetters::ConvertNumber(const std::string& str)
{
	return atoi(str.c_str());
}

int	ConfigSetters::ConvertSize(const std::string& str)
{
	int	result = atoi(str.c_str());
	int	mult = CheckSize(*(str.rbegin()), SIZETYPE_BYTES);

	if (mult != VER_ERROR)
		return result * mult;
	return result;
}

int	ConfigSetters::ConvertTime(const std::string& str)
{
	int	result = atoi(str.c_str());
	int	mult = CheckSize(*(str.rbegin()), SIZETYPE_TIME);

	if (mult != VER_ERROR)
		return result * mult;
	return result;
}

std::string	ConfigSetters::ConvertDirectory(const std::string& str)
{
	return str;
}

std::string	ConfigSetters::ConvertFilePath(const std::string& str)
{
	return str;
}

std::string	ConfigSetters::ConvertURL(const std::string& str)
{
	return str;
}

std::string	ConfigSetters::ConvertMethod(const std::string& str)
{
	return str;
}

IpPort	ConfigSetters::ConvertIP(const std::string& str)
{
	IpPort				result(INADDR_ANY, DEF_PORT);
	std::stringstream	buffer(str);
	int					nextDigit;
	char				point;

	if (VerifyNumber(str))
		return IpPort(INADDR_ANY, ConvertNumber(str));
	for (int i = 0; i < 4; i++)
	{
		buffer >> nextDigit;
		result.first <<= 8;
		result.first += nextDigit;
		point = buffer.get();
	}
	if (point == ':')
		buffer >> result.second;
	return result;
}
