/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 21:10:46 by glemaire          #+#    #+#             */
/*   Updated: 2024/06/03 21:49:56 by glemaire         ###   ########.fr       */
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


char	*get_str(t_ast *c, int active)
{
	char	*str;

	str = ft_strdup("");
	c = c->right;
	while (c)
	{
		str = gnl_strjoin(str, c->str);
		if (c->right)
			str = gnl_strjoin(str, " ");
		c = c->right;
	}
	if (!active)
		str = gnl_strjoin(str, "\n");
	return (str);
}

int	get_option(t_ast *c)
{
	int	option;
	
	option = 0;
	c = c->right;
	while (c && check_option(c->str))
	{
		option = 1;
		c = c->right;
	}
	return (option);
}

void	builtin_echo(t_data *data, t_ast *c)
{
	int		option;
	char	*str;

	option = get_option(c);
	str = get_str(c, option);
	if (!str)
	{
		if (c->prev_node == PIPE)
			data_destroy_exit(data, EXIT_FAILURE, "str2", strerror(ENOMEM));
		else
			err_message(data, EXIT_FAILURE, "str", strerror(ENOMEM));
		return ;
	}
	ft_putstr_fd(str, data->out);
	// close_out(data);
	free(str);
	data->exit = EXIT_SUCCESS;
	// data_destroy_exit(data, EXIT_SUCCESS, NULL, NULL);
}
