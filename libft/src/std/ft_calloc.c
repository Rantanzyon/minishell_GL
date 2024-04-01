/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 15:39:01 by glemaire          #+#    #+#             */
/*   Updated: 2023/10/25 22:48:35 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*str;
	size_t	prod;

	if (size == 0 || nmemb == 0)
		return (malloc(0));
	prod = size * nmemb;
	if (prod / size != nmemb)
		return (NULL);
	str = (void *)malloc(prod);
	if (!str)
		return (NULL);
	ft_bzero(str, prod);
	return (str);
}
