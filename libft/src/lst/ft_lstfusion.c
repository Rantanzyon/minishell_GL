/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstfusion_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 23:20:12 by glemaire          #+#    #+#             */
/*   Updated: 2024/03/24 23:20:40 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstfusion(t_list **a, t_list **b)
{
	t_list	*c;

	if (!a || !(*a) || !b || !(*b))
		return ;
	c = *a;
	while (c->next)
		c = c->next;
	c->next = *b;
	free(b);
}
