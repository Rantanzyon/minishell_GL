/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 00:23:44 by glemaire          #+#    #+#             */
/*   Updated: 2023/10/29 13:35:30 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isneg(int n)
{
	if (n < 0)
		return (1);
	return (0);
}

static int	ft_digit(int n)
{
	int	i;

	i = 1;
	while (n > 9)
	{
		i++;
		n = n / 10;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	int		sign;
	char	*str;
	int		unit;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	sign = ft_isneg(n);
	if (n < 0)
		n = n * -1;
	unit = ft_digit(n);
	str = malloc((unit + sign + 1) * sizeof(char));
	if (!str)
		return (NULL);
	if (sign)
		str[0] = '-';
	str[unit + sign] = '\0';
	while (unit > 0)
	{
		str[unit + sign - 1] = n % 10 + '0';
		n = n / 10;
		unit--;
	}
	return (str);
}
