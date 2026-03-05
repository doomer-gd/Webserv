/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 17:16:19 by ikulik            #+#    #+#             */
/*   Updated: 2026/03/05 16:56:44 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_CONFIG_HPP
# define PARSE_CONFIG_HPP
# include <string.h>
# include "../main/main.hpp"
# define VER_ERROR -1
# define CONF_URI_CHARS "-_.~/"

const std::set<std::string> g_supported_methods
{
	"GET",
	"POST",
	"DELETE"
};

enum ESizeType
{
	SIZETYPE_BYTES,
	SIZETYPE_TIME
};

struct Size
{
	char	param;
	int		size;
};

const struct Size g_memory_formats[]
{
	{'k', 1024},
	{'m', 1048576},
};

const struct Size g_time_formats[]
{
	{'s', 1},
	{'m', 60},
	{'h', 3600},
	{'d', 86400},
	{'y', 31536000}
};

typedef std::string::iterator IterStr;
typedef std::vector<std::string> LineArray;
typedef std::pair<unsigned int, int> IpPort;


//general workflow: get next token, get its args, use the setter from the dictionary
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

enum EConfigDict
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
//write into respective block
//if new scope appears, call a setter to change it
//on setter error - return error
class ConfigSetters
{
	private:
		EConfigDict		currentScope;
		Heirarchy		scopeHier;
		ConfigMain*		config;
		ServerConfig*	currentServer;
		LocationConfig*	currentLocation;


		void	SetUpScopeHeirarchy(void);
		int	SetScope(LineArray& args, EConfigDict scopeNew);
		template<typename T>
		int	VerifySingleParam(LineArray& args, EConfigDict scope, bool (*verify)(const std::string&));
		template<typename T>
		int	VerifyMultipleParam(LineArray& args, EConfigDict scope, bool (*verify)(const std::string&));
		template<typename T>
		int	SetSingleParam(T& param, LineArray& args, EConfigDict scope, bool (*verify)(const std::string&), T (*convert)(const std::string&));
		template<typename T, typename L>
		int	SetMultipleParam(T& param, LineArray& args, EConfigDict scope, bool (*verify)(const std::string&), L (*convert)(const std::string&));
	public:
		ConfigSetters();
		//main scope
		int	SetErrorLog(LineArray& args);
		int	SetEvents(LineArray& args);
		int	SetFdsMax(LineArray& args);
		int	SetHeaderBufferSize(LineArray& args);
		int	SetBodyBufferSize(LineArray& args);
		int	SetHttp(LineArray& args);
		//events
		int	SetMaxConnections(LineArray& args);
		//http
		int	SetHeaderTimeout(LineArray& args);
		int	SetBodyTimeout(LineArray& args);
		int	SetKeepAliveTimeout(LineArray& args);
		int	SetSendTimeout(LineArray& args);
		int	SetGeneralTimeout(LineArray& args);
		int	SetServer(LineArray& args);
		//server
		int	SetServerName(LineArray& args);
		int	SetErrorPages(LineArray& args);
		int	SetListen(LineArray& args);
		int	SetMaxBodySize(LineArray& args);
		int	SetLocation(LineArray& args);
		//location
		int	SetRoot(LineArray& args);
		int	SetIndex(LineArray& args);
		int	SetAutoindex(LineArray& args);
		int	SetMethods(LineArray& args);
		int	SetRedirect(LineArray& args);
		int	SetUploadStore(LineArray& args);
		int	SetCgi(LineArray& args);

		static int	CheckSize(char literal, ESizeType mode);
		static bool	IsUriChar(char ch);
		static bool	VerifyNumber(const std::string& str);
		static bool	VerifySize(const std::string& str);
		static bool	VerifyTime(const std::string& str);
		static bool	VerifyDirectory(const std::string& str);
		static bool	VerifyFilePath(const std::string& str);
		static bool	VerifyURL(const std::string& str);
		static bool	VerifyMethod(const std::string& str);
		static bool	VerifyIP(const std::string& str);
		static bool	VerifyExtension(const std::string& str);

		static int			ConvertNumber(const std::string& str);
		static int			ConvertSize(const std::string& str);
		static int			ConvertTime(const std::string& str);
		static std::string	ConvertDirectory(const std::string& str);
		static std::string	ConvertFilePath(const std::string& str);
		static std::string	ConvertURL(const std::string& str);
		static std::string	ConvertMethod(const std::string& str);
		static IpPort		ConvertIP(const std::string& str);
};

class ConfigParser
{
	private:
		ConfigSetters	setter;
		ConfigMain		config;
		std::ifstream	fileInput;
		std::istream&	fileStream = fileInput;
		std::map<std::string, int (ConfigSetters::*)(LineArray&)>	dicts[CD_NUM_DICTS];
		std::string					field;
		std::vector<std::string>	args;

		int		OpenFile(const std::string& fileName);
		void	SetUpDictionaries();

		//used to check that args are ok
		int	ParseMainBody(const std::string& fileName);
	public:
		int	GetConfig(ConfigMain& config, const std::string& fileName);


};

template<typename T>
int	ConfigSetters::VerifySingleParam(LineArray& args, EConfigDict scope, bool (*verify)(const std::string&))
{
	if (currentScope != scope || args.size() != 1)
		return E_FAILURE;
	if ((*verify)(args[0]) == false)
		return E_FAILURE;
	return E_SUCCESS;
}

template<typename T>
int	ConfigSetters::VerifyMultipleParam(LineArray& args, EConfigDict scope, bool (*verify)(const std::string&))
{
	int	size = args.size();
	if (currentScope != scope || size == 0)
		return E_FAILURE;
	for (int i = 0; i < size; i++)
	{
		if ((*verify)(args[i]) == false)
			return E_FAILURE;
	}
	return E_SUCCESS;
}

template<typename T>
int	ConfigSetters::SetSingleParam(T& param, LineArray& args, EConfigDict scope, bool (*verify)(const std::string&), T (*convert)(const std::string&))
{
	if (VerifySingleParam(args, scope, verify) == E_FAILURE)
		return E_FAILURE;
	param = (*convert)(args[0]);
	return E_SUCCESS;
}

template<typename T, typename L>
int	ConfigSetters::SetMultipleParam(T& param, LineArray& args, EConfigDict scope, bool (*verify)(const std::string&), L (*convert)(const std::string&))
{
	int	size = args.size();

	if (VerifyMultipleParam(args, scope, verify) == false)
		return E_FAILURE;
	for (int i = 0; i < size; i++)
		param.insert(param.end(), (*convert)(args[i]));
	return E_SUCCESS;
}

#endif
