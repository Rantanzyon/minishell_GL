/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_insert_lst.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 00:12:54 by glemaire          #+#    #+#             */
/*   Updated: 2024/03/25 00:14:58 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	insert_lst(t_list **a, t_list **b, int to)
{
	int		i;
	t_list	*prev;
	t_list	*c;

	c = *a;
	prev = *a;
	i = 0;
	while (c)
	{
		if (i == to)
		{
			prev->next = *b;
			ft_lstlast(*b)->next = c;
			break ;
		}
		prev = c;
		c = c->next;
		i++;
	}
}

void	ft_lst_insert_lst(t_list **a, t_list **b, int to)
{
	if (!a || !(*a) || !b || !(*b))
		return ;
	if (to == 0)
	{
		ft_lstlast(*b)->next = *a;
		*a = *b;
	}
	else if (to >= ft_lstsize(*a) - 1 || to < 0)
		ft_lstlast(*a)->next = *b;
	else
		insert_lst(a, b, to);
	free(b);
}
