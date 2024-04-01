/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmove_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 22:09:25 by glemaire          #+#    #+#             */
/*   Updated: 2024/03/24 23:42:02 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_list	*bridge(t_list **a, int from)
{
	int		i;
	t_list	*cursor;
	t_list	*temp1;
	t_list	*start;

	i = 0;
	cursor = *a;
	temp1 = *a;
	while (cursor)
	{
		if (i == from)
		{
			temp1->next = cursor->next;
			start = cursor;
		}
		temp1 = cursor;
		cursor = cursor->next;
		i++;
	}
	return (start);
}

static t_list	*extract_node(t_list **a, int from)
{
	t_list	*cursor;
	t_list	*start;

	if (from == 0)
	{
		start = *a;
		*a = (*a)->next;
	}
	else if (from == -1)
	{
		cursor = *a;
		while (cursor)
		{
			if (cursor->next->next == NULL)
			{
				start = cursor->next;
				cursor->next = NULL;
			}
			cursor = cursor->next;
		}
	}
	else
		start = bridge(a, from);
	return (start);
}

void	ft_lstmove(t_list **a, int from, int to)
{
	t_list	*start;

	if (!a || !(*a))
		return ;
	if (from >= ft_lstsize(*a) - 1 || from < 0)
		from = -1;
	if (to >= ft_lstsize(*a) - 1 || to < 0)
		to = -1;
	if (from == to)
		return ;
	start = extract_node(a, from);
	ft_lstadd_at(a, start, to);
}
