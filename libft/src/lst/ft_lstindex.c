/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstindex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 11:06:39 by glemaire          #+#    #+#             */
/*   Updated: 2024/03/30 14:09:49 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstindex(t_list **a, t_list *c)
{
	t_list	*b;
	t_list	*d;

	b = *a;
	d = c;
	if (!c)
		return (ft_lstsize(b));
	return (ft_lstsize(b) - ft_lstsize(d));
}
