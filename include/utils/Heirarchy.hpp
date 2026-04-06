/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Heirarchy.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikulik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 14:31:59 by ikulik            #+#    #+#             */
/*   Updated: 2026/03/05 14:42:00 by ikulik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEIRARCHY_HPP
# define HEIRARCHY_HPP
# include <map>
# define LOOKUP_FAIL -1

class Heirarchy
{
	private:
		std::map<int, int>	parentMap;
	public:
		Heirarchy();
		void	AddParent(int child, int parent);
		int		GetParent(int child);
};


#endif