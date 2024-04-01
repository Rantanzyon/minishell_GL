/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelnode.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 22:51:34 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/01 20:29:48 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	bridge(t_list **a, int index)
{
	int		i;
	t_list	*cursor;
	t_list	*temp;

	i = 0;
	cursor = *a;
	temp = *a;
	while (cursor)
	{
		if (i == index)
		{
			temp->next = cursor->next;
			free(cursor);
			return ;
		}
		temp = cursor;
		cursor = cursor->next;
		i++;
	}
}

void	ft_lstdelnode(t_list **a, int index)
{
	t_list	*cursor;

	if (!a || !(*a) || index > ft_lstsize(*a) - 1 || index < -1)
		return ;
	if (index == 0)
	{
		cursor = (*a)->next;
		free(*a);
		*a = cursor;
	}
	else if (index == -1 || index == ft_lstsize(*a) - 1)
	{
		cursor = *a;
		while (cursor)
		{
			if (cursor->next->next == NULL)
			{
				free(cursor->next);
				cursor->next = NULL;
			}
			cursor = cursor->next;
		}
	}
	else
		bridge(a, index);
}
