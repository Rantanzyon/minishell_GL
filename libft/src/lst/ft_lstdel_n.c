/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel_n.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 16:34:44 by glemaire          #+#    #+#             */
/*   Updated: 2024/03/31 16:51:28 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel_n(t_list **a, int len, int index)
{
	int	j;

	j = 0;
	while (j < len)
	{
		ft_lstdelnode(a, index);
		j++;
	}
}
