/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Heirarchy.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 14:35:25 by ikulik            #+#    #+#             */
/*   Updated: 2026/03/05 14:42:18 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/utils/Heirarchy.hpp"

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