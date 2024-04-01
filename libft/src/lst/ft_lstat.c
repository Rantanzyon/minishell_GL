/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstat.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 11:06:01 by glemaire          #+#    #+#             */
/*   Updated: 2024/03/30 14:09:43 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstat(t_list **a, int index)
{
	int		i;
	t_list	*c;

	i = 0;
	c = *a;
	while (c)
	{
		if (i == index)
			return (c);
		c = c->next;
		i++;
	}
	return (*a);
}
