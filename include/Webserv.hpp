/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 15:18:43 by ikulik            #+#    #+#             */
/*   Updated: 2025/12/02 16:45:24 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "main.hpp"

class	Webserv
{
	private:
		static int	exitCode_;
	public:
		static int	Exit(ExitCode errorCode);
		Webserv(){};
		~Webserv(){};
};

#endif
