/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Basics.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:36:46 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/18 17:51:47 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASICS_H
# define BASICS_H
# include "main.hpp"

void	ft_bzero(void *s, size_t n);

template<class T>
void	safeDelete(T* obj)
{
	if (obj != NULL)
		delete obj;
}

template <typename T>
std::string toString(T value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

#endif



