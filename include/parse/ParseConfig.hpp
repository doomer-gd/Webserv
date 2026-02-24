/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 17:16:19 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/24 21:41:41 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_CONFIG_HPP
# define PARSE_CONFIG_HPP
# include "../main/main.hpp"

class ConfigParser
{
	private:
		Config			config;
		std::ifstream	fileInput;
		std::string		field;
		std::string		value;

		int	OpenFile(const std::string& fileName);
	public:
		int				ParseConfigFile(const std::string& fileName);
		const Config&	GetConfig() const;
};


#endif
