/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 00:01:52 by glemaire          #+#    #+#             */
/*   Updated: 2023/10/26 15:43:18 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize != 0)
	{
		while (*(src + i) && (i < dstsize - 1))
		{
			*(char *)(dst + i) = *(char *)(src + i);
			i++;
		}
		*(char *)(dst + i) = '\0';
	}
	while (*(src + i))
		i++;
	return (i);
}
