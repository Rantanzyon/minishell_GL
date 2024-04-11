/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 21:10:46 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/10 21:26:23 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_option(char *str)
{
	int	i;
	
	if (str[0] != '-')
		return (0);
	else
	{
		i = 1;
		while (str[i])
		{
			if (str[i] != 'n')
				return (0);
			i++;
		}
	}
	return (1);
}

void	builtin_echo(t_data *data, t_ast *c)
{
	int		active;
	char	*str;

	active = 0;
	str = ft_strdup("");
	if (!str)
		data_destroy_exit(data, EXIT_FAILURE, "str");
	if (c->right)
		c = c->right;
	while (c && check_option(c->str))
	{
		active = 1;
		c = c->right;
	}
	//next
	
}
