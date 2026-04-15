/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 17:16:19 by ikulik            #+#    #+#             */
/*   Updated: 2026/04/15 17:10:03 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP
# include "parse/ConfigDefines.hpp"
# include "main/Config.hpp"
# include <fstream>

//general workflow: get next token, get its args, use the setter from the dictionary
//if key not found = return error, otherwise continue
//if token ends scope, return to previous scope in the heirarchy


//track the scope inside ConfigSetter
//write into respective block
//if new scope appears, like a field "server", check if it fits the heirarchy, if ok add new item
//like a new server or location
//on setter error - return error

class ConfigSetters;
class ConfigTokenizer;

class ConfigParser
{
	private:
		ConfigSetters*		setter;
		ConfigTokenizer*	tokenizer;
		std::ifstream		fileInput;
		std::istream&		fileStream;
		std::string			buffer;

		int		OpenFile(const std::string& fileName);
		int		ExitParser(int exitCode);
		int		GetArguments(LineArray& args);
		bool	IsEndingArgument(const std::string& arg);
	public:
		ConfigParser();
		~ConfigParser();
		int	ParseConfigFile(ConfigMain& config, const char* fileName);
		const std::string&	GetErrorLine(void) const;
};

#endif
