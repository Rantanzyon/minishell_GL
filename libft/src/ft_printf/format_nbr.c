/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_nbr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 01:51:42 by glemaire          #+#    #+#             */
/*   Updated: 2024/03/30 06:16:13 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_nbrlen(unsigned long long n, unsigned int base)
{
	int	count;

	count = 1;
	while (n > base - 1)
	{
		count++;
		n = n / base;
	}
	return (count);
}

static char	*ft_itoa_base(
	unsigned long long n, char *baselist, unsigned int base, int extra)
{
	char	*str;
	int		sizenb;

	sizenb = ft_nbrlen(n, base);
	str = malloc((sizenb + extra + 1) * sizeof(char));
	if (!str)
		return (NULL);
	if (extra == 1)
		str[0] = '-';
	if (extra == 2)
	{
		str[0] = '0';
		str[1] = 'x';
	}
	str[sizenb + extra] = '\0';
	while (sizenb > 0)
	{
		str[sizenb + extra - 1] = baselist[n % base];
		n = n / base;
		sizenb--;
	}
	return (str);
}

int	checknbr(unsigned long long n, char c, int extra)
{
	char	*str;
	int		len;

	len = 0;
	if (c == 'd' || c == 'i')
		str = ft_itoa_base(n, "0123456789", 10, extra);
	if (c == 'x')
		str = ft_itoa_base(n, "0123456789abcdef", 16, 0);
	if (c == 'X')
		str = ft_itoa_base(n, "0123456789ABCDEF", 16, 0);
	if (c == 'u')
		str = ft_itoa_base(n, "0123456789", 10, 0);
	if (c == 'p')
		str = ft_itoa_base(n, "0123456789abcdef", 16, 2);
	if (!str)
		return (0);
	len += ft_putstr(str);
	free(str);
	return (len);
}
