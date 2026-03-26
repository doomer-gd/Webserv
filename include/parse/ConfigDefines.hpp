/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigDefines.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 18:00:53 by ikulik            #+#    #+#             */
/*   Updated: 2026/03/26 19:13:22 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_DEFINES_HPP
#define CONFIG_DEFINES_HPP
# include <vector>
# include <string>
# include <set>

enum EPasreState
{
	EPS_REGULAR,
	EPS_DOUBLE_QUOTE,
	EPS_SINGLE_QUOTE,
	EPS_COMMENT,
	EPS_ERROR,
	EPS_NUM_STATES
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

struct Size
{
	char	param;
	int		size;
	// Size(char param, int size): param(param), size(size){};
};

typedef std::string::iterator IterStr;
typedef std::vector<std::string> LineArray;
typedef std::pair<unsigned int, int> IpPort;

# define VER_ERROR -1
# define CONF_URI_CHARS "-_.~/"
# define CONF_TOKEN_ENDERS ";{}"
# define NUM_SUP_METHODS 3

extern const std::string g_supported_methods[];

extern const struct Size	g_memory_formats[];
extern const int			g_memory_formats_size;

extern const struct Size	g_time_formats[];
extern const int 			g_time_formats_size;

enum ESizeType
{
	SIZETYPE_BYTES,
	SIZETYPE_TIME
};

#endif
