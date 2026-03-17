/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtrofyme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 00:00:00 by vtrofyme          #+#    #+#             */
/*   Updated: 2026/03/16 00:00:00 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <string>
# include <vector>
# include <fstream>
# include <sstream>
# include <stdexcept>
# include "../main/Config.hpp"

class ConfigParser
{
	private:
		std::vector<std::string>	tokens;
		size_t						pos;

		void			tokenize(const std::string& content);
		ServerConfig	parseServerBlock();
		LocationConfig	parseLocationBlock();
		size_t			parseSize(const std::string& sizeStr);
		void			expect(const std::string& expected);
		std::string		next();
		std::string		peek() const;
		bool			hasMore() const;

	public:
		ConfigParser();
		~ConfigParser();

		Config	parse(const std::string& filepath);
};

#endif
