/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel_here.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 17:45:51 by glemaire          #+#    #+#             */
/*   Updated: 2024/03/31 14:10:36 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstdel_here(t_list **a, t_list *c)
{
	t_list	*next;
	int		index;

	index = ft_lstindex(a, c);
	ft_lstdelnode(a, index);
	next = ft_lstat(a, index);
	return (next);
}
