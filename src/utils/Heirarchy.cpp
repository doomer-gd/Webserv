/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Heirarchy.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 14:35:25 by ikulik            #+#    #+#             */
/*   Updated: 2026/03/20 12:26:34 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/Heirarchy.hpp"

Heirarchy::Heirarchy(){};

void	Heirarchy::AddParent(int child, int parent)
{
	parentMap[child] = parent;
}

int	Heirarchy::GetParent(int child)
{
	std::map<int, int>::iterator	find;

	find  = parentMap.find(child);
	if (find == parentMap.end())
		return LOOKUP_FAIL;
	return find->second;
}
