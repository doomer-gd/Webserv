/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigSetters.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 17:10:21 by ikulik            #+#    #+#             */
/*   Updated: 2026/03/11 17:07:10 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_SETTER_HPP
# define CONFIG_SETTER_HPP
# include "ConfigDefines.hpp"
# include "../main/Config.hpp"
# include "../utils/Heirarchy.hpp"

class ConfigSetters
{
	public:
		typedef bool (*Verifier)(const std::string&);
		typedef int (ConfigSetters::*Setter)(LineArray&);
		typedef std::map<std::string, int (ConfigSetters::*)(LineArray&)> Dictionary;
	private:
		Dictionary		dicts[CD_NUM_DICTS];
		EConfigDict		currentScope;
		Heirarchy		scopeHier;
		Setter			currentSetter;
		ConfigMain*		config;
		ServerConfig*	currentServer;
		LocationConfig*	currentLocation;

		void	SetUpScopeHeirarchy(void);
		void	SetUpDictionaries(void);
		int		SetScope(LineArray& args, EConfigDict scopeNew);
		//calls selecter verifier
		int	VerifySingleParam(LineArray& args, EConfigDict scope, Verifier verify);
		int	VerifyMultipleParam(LineArray& args, EConfigDict scope, Verifier verify);
		//calls verifier, then setter
		template<typename T>
		int	SetSingleParam(T& param, LineArray& args, EConfigDict scope, Verifier verify, T (*convert)(const std::string&));
		template<typename T, typename L>
		int	SetMultipleParam(T& param, LineArray& args, EConfigDict scope, Verifier verify, L (*convert)(const std::string&));

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
	public:
		ConfigSetters(ConfigMain& config);
		int	SelectSetter(const std::string& nameParameter);
		int	SetParameter(LineArray& args);

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

template<typename T>
int	ConfigSetters::SetSingleParam(T& param, LineArray& args, EConfigDict scope, Verifier verify, T (*convert)(const std::string&))
{
	if (VerifySingleParam(args, scope, verify) == E_FAILURE)
		return E_FAILURE;
	param = (*convert)(args[0]);
	return E_SUCCESS;
}

template<typename T, typename L>
int	ConfigSetters::SetMultipleParam(T& param, LineArray& args, EConfigDict scope, Verifier verify, L (*convert)(const std::string&))
{
	int	size = args.size();

	if (VerifyMultipleParam(args, scope, verify) == false)
		return E_FAILURE;
	for (int i = 0; i < size; i++)
		param.insert(param.end(), (*convert)(args[i]));
	return E_SUCCESS;
}

#endif
