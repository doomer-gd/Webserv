/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 17:16:19 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/26 20:47:52 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_CONFIG_HPP
# define PARSE_CONFIG_HPP
# include "../main/main.hpp"

typedef std::string::iterator IterStr;

//general workflow: get next token, get its arguments, use the setter from the dictionary
//if key not found = return error, otherwise continue
//if token ends scope, return to previous scope and set value

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
	CD_MAIN,
	CD_EVENTS,
	CD_HTTP,
	CD_SERVER,
	CD_LOCATION,
	CD_ERROR,
	CD_NUM_DICTS
};

//probably unnecessary
class AParseState
{
	private:
		const char*	expected;
		bool	CheckExpected(char c); //make it inline
	public:
		virtual StateStatus	InspectNext(std::istream& file, std::string& buffer);
		virtual EPasreState	Exit();
};

//track the scope inside ConfigSetter
//for directives with blocks: if the scope is the same = create object, return to previous scope
//on setter error - return error
class ConfigSetters
{
	private:
		EConfigDictionary	currentScope;
	public:
		//main scope
		EConfigDictionary	SetErrorLog(ConfigItem* object, void* arguments);
		EConfigDictionary	SetEvents(ConfigItem* object, void* arguments);
		EConfigDictionary	SetUserGroup(ConfigItem* object, void* arguments);
		EConfigDictionary	SetFdsMax(ConfigItem* object, void* arguments);
		EConfigDictionary	SetHeaderBufferSize(ConfigItem* object, void* arguments);
		EConfigDictionary	SetBodyBufferSize(ConfigItem* object, void* arguments);
		//events
		EConfigDictionary	SetMaxConnections(ConfigItem* object, void* arguments);
		//http
		EConfigDictionary	SetHeaderTimeout(ConfigItem* object, void* arguments);
		EConfigDictionary	SetBodyTimeout(ConfigItem* object, void* arguments);
		EConfigDictionary	SetKeepAliveTimeout(ConfigItem* object, void* arguments);
		EConfigDictionary	SetSendTimeout(ConfigItem* object, void* arguments);
		EConfigDictionary	SetGeneralTimeout(ConfigItem* object, void* arguments);
		EConfigDictionary	SetServer(ConfigItem* object, void* arguments);
		//server
		EConfigDictionary	SetServerName(ConfigItem* object, void* arguments);
		EConfigDictionary	SetErrorPages(ConfigItem* object, void* arguments);
		EConfigDictionary	SetListen(ConfigItem* object, void* arguments);
		EConfigDictionary	SetMaxBodySize(ConfigItem* object, void* arguments);
		EConfigDictionary	SetLocation(ConfigItem* object, void* arguments);
		//location
		EConfigDictionary	SetRoot(ConfigItem* object, void* arguments);
		EConfigDictionary	SetIndex(ConfigItem* object, void* arguments);
		EConfigDictionary	SetAutoindex(ConfigItem* object, void* arguments);
		EConfigDictionary	SetMethods(ConfigItem* object, void* arguments);
		EConfigDictionary	SetRedirect(ConfigItem* object, void* arguments);
		EConfigDictionary	SetUploadStore(ConfigItem* object, void* arguments);
		EConfigDictionary	SetCgiPath(ConfigItem* object, void* arguments);
};

class ConfigParser
{
	private:
		ConfigSetters	setter;
		ConfigMain		config;
		std::ifstream	fileInput;
		std::istream&	fileStream = fileInput;
		std::map<std::string, EConfigDictionary (ConfigSetters::*)(ConfigItem*, void*)>	dicts[CD_NUM_DICTS];
		std::string					field;
		std::vector<std::string>	arguments;

		int		OpenFile(const std::string& fileName);
		void	SetUpDictionaries();

		//used to check that arguments are ok
		static int	VerifyNumber(const std::string& str);
		static bool	VerifyIP(const std::string& str);
		static int	VerifySize(const std::string& str);
		static int	VerifyTime(const std::string& str);
		static bool	VerifyDirection(const std::string& str);
	
		int	ParseMainBody(const std::string& fileName);
	public:
		int	GetConfig(ConfigMain& config, const std::string& fileName);
};



#endif
