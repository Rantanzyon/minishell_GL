/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 14:29:57 by glemaire          #+#    #+#             */
/*   Updated: 2023/10/29 17:56:15 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	dstlen;
	size_t	srclen;

	if (!dst && src && dstsize == 0)
		return (ft_strlen(src));
	dstlen = ft_strlen(dst);
	srclen = ft_strlen(src);
	i = 0;
	while (dst[i])
		i++;
	j = 0;
	if (dstsize > dstlen)
	{
		while (src[j] && j < dstsize - dstlen - 1)
			dst[i++] = src[j++];
		dst[i] = '\0';
		return (srclen + dstlen);
	}
	return (srclen + dstsize);
}
