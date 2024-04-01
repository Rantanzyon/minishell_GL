/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 10:57:00 by glemaire          #+#    #+#             */
/*   Updated: 2024/03/30 08:40:03 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_nextchar(char c, va_list arg)
{
	int	n;

	if (c == 'c')
		return (ft_putchar(va_arg(arg, int)));
	else if (c == 's')
		return (ft_putstr(va_arg(arg, char *)));
	else if (c == '%')
		return (write(1, "%", 1));
	else if (c == 'd' || c == 'i')
	{
		n = va_arg(arg, int);
		if (n < 0)
			return (checknbr(-(unsigned long long)n, c, 1));
		return (checknbr((unsigned long long)n, c, 0));
	}
	else if (c == 'x' || c == 'X' || c == 'u')
		return (checknbr((unsigned long long)va_arg(arg, unsigned int), c, 0));
	else if (c == 'p')
		return (checknbr(va_arg(arg, unsigned long long), c, 2));
	return (0);
}

int	ft_printf(const char *str, ...)
{
	int		len;
	int		i;
	va_list	arg;

	if (!str)
		return (-1);
	va_start(arg, str);
	len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '%')
		{
			if (ft_strchr("cspdiuxX%", str[i + 1]))
				len += check_nextchar(str[i++ + 1], arg);
		}
		else
			len += write(1, &str[i], 1);
		i++;
	}
	va_end (arg);
	return (len);
}
