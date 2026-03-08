/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Basics.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:35:58 by ikulik            #+#    #+#             */
/*   Updated: 2026/02/18 17:51:42 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/utils/Basics.hpp"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*dest;
	size_t			i;

	i = 0;
	dest = (unsigned char *)s;
	while (i < n)
	{
		*dest = '\0';
		i++;
		dest++;
	}
}
