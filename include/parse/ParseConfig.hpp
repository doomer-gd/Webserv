/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 17:16:19 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/27 18:28:51 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_CONFIG_HPP
# define PARSE_CONFIG_HPP
# include "../main/main.hpp"

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
		ConfigMain*		config;
		ServerConfig*	currentServer;
		LocationConfig*	currentLocation;

		int SetInt(int& var, LineArray& args, EConfigDict scope, int (ConfigSetters::*setter)(const std::string&));
		int SetUri(std::string& var, LineArray& args, EConfigDict scope);
		int SetScope(LineArray& args, EConfigDict scopeNew, EConfigDict scopeParent);
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
		int	SetCgiPath(LineArray& args);

		static int	VerifyNumber(const std::string& str);
		static int	VerifySize(const std::string& str);
		static int	VerifyTime(const std::string& str);
		static int	VerifyDirectory(const std::string& str);
		static int	VerifyURL(const std::string& str);
		static IpPort	VerifyIP(const std::string& str);
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



#endif
