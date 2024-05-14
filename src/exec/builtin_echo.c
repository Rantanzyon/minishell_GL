/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 21:10:46 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/14 19:16:09 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_option(char *str)
{
	int	i;

	if (str[0] != '-' || !ft_strcmp(str, "-"))
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

char	*get_str(t_data *data, t_ast *c)
{
	char	*str;

	str = ft_strdup("");
	if (!str)
		data_destroy_exit(data, EXIT_FAILURE, "str", strerror(ENOMEM));
	while (c)
	{
		str = gnl_strjoin(str, c->str);
		if (c->right)
			str = gnl_strjoin(str, " ");
		if (!str)
			data_destroy_exit(data, EXIT_FAILURE, "str", strerror(ENOMEM));
		c = c->right;
	}
	return (str);
}

void	builtin_echo(t_data *data, t_ast *c)
{
	int		active;
	char	*str;

	active = 0;
	c = c->right;
	while (c && check_option(c->str))
	{
		active = 1;
		c = c->right;
	}
	str = get_str(data, c);
	if (!active)
	{
		str = gnl_strjoin(str, "\n");
		if (!str)
			data_destroy_exit(data, EXIT_FAILURE, "str", strerror(ENOMEM));
	}
	ft_putstr_fd(str, data->fd_out);
	close(data->fd_out);
	free(str);
	data_destroy_exit(data, EXIT_SUCCESS, NULL, NULL);
}
