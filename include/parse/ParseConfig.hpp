/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 17:16:19 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/25 20:04:10 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_CONFIG_HPP
# define PARSE_CONFIG_HPP
# include "../main/main.hpp"

typedef std::string::iterator IterStr;

enum EPasreState
{
	PS_REGULAR,
	PS_DOUBLE_QUOTE,
	PS_SINGLE_QUOTE,
	PS_COMMENT,
	PS_NUM_STATES
};

enum EConfigDictionary
{
	DC_MAIN_BODY,
	DC_EVENTS,
	DC_HTTP,
	DC_SERVER,
	DC_LOCATION,
	DC_NUM_DICTS
};

class AParseState
{
	private:
		const char*	expected;
		inline bool	CheckExpected(char c);
	public:
		virtual StateStatus	InspectNext(std::istream& file, std::string& buffer);
		virtual EPasreState	Exit();
};

class ConfigParser
{
	private:
		Config			config;
		std::ifstream	fileInput;
		std::istream&	fileStream = fileInput;
		std::map<std::string&, void (*)(std::string& value, void* object)>	dicts[DC_NUM_DICTS];
		std::string		field;
		std::string		value;

		inline int	OpenFile(const std::string& fileName);
		static bool	VerifyNumber(const std::string& str);
		void		SetUpDictionaries();
		int			ParseMainBody(const std::string& fileName);
		int			ParseHTTP();
		int			ParseServer(ServerConfig& config);
		int			ParseLocation(LocationConfig& location);
	public:
		int	GetConfig(Config& config, const std::string& fileName);
};



#endif
