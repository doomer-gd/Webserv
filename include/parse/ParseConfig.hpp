/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 17:16:19 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/24 17:46:04 by ikulik           ###   ########.fr       */
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
	public:
		int				ParseConfigFile(const std::string& fileName);
		const Config&	GetConfig() const;
}


#endif
