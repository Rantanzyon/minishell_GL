/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_at.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 23:37:55 by glemaire          #+#    #+#             */
/*   Updated: 2024/03/31 15:26:54 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	lstadd_middle(t_list **a, t_list *start, int to)
{
	t_list	*cursor;
	t_list	*temp1;
	int		i;

	cursor = *a;
	i = 0;
	temp1 = cursor;
	while (cursor)
	{
		if (i == to)
		{
			temp1->next = start;
			start->next = cursor;
		}
		temp1 = cursor;
		cursor = cursor->next;
		i++;
	}
}

void	ft_lstadd_at(t_list **a, t_list *start, int to)
{
	if (!a || !start)
		return ;
	if (!(*a) || to == 0)
		ft_lstadd_front(a, start);
	else if (to == -1 || to > ft_lstsize(*a) - 1)
	{
		start->next = NULL;
		ft_lstadd_back(a, start);
	}
	else
		lstadd_middle(a, start, to);
}
