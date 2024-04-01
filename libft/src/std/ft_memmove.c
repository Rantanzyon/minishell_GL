/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 17:42:33 by glemaire          #+#    #+#             */
/*   Updated: 2023/10/26 15:22:54 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	i;

	if (!dst && !src)
		return (NULL);
	i = 0;
	while (i < len)
	{
		if (src <= dst)
			*((char *)dst + (len - 1) - i) = *((char *)src + (len - 1) - i);
		else
			*((char *)dst + i) = *((char *)src + i);
		i++;
	}
	return (dst);
}
