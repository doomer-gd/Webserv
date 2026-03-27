/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Basics.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:36:46 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/22 00:00:00 by vtrofyme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASICS_HPP
# define BASICS_HPP

# include <stdlib.h>
# include <string>
# include <sstream>

void	ft_bzero(void *s, size_t n);

template<class T>
void	safeDelete(T** obj)
{
	if (obj == NULL)
		return ;
	if (*obj != NULL)
		delete *obj;
	*obj = NULL;
}

template <typename T, typename L>
std::ostream& operator<<(std::ostream& os, const std::pair<T, L>& pair)
{
	os << pair.first << " " << pair.second;
	return os;
}

template <typename T>
std::string toString(T value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}



#endif
