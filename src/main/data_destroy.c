/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_destroy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:24:57 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/21 19:05:40 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	err_message(t_data *data, char *name, char *err)
{
	char	*str;

	str = ft_strjoin(RED, "minishell: ");
	if (name)
	{
		str = gnl_strjoin(str, name);
		str = gnl_strjoin(str, ": ");
	}
	str = gnl_strjoin(str, err);
	str = gnl_strjoin(str, DEF);
	str = gnl_strjoin(str, "\n");
	if (!str)
	{
		ft_putstr_fd(strerror(ENOMEM), STDERR_FILENO);
		data_destroy_exit(data, EXIT_FAILURE, NULL, NULL);
	}
	ft_putstr_fd(str, STDERR_FILENO);
	free(str);
}

void	data_destroy(t_data *data, char *name, char *err)
{
	if (err)
		err_message(data, name, err);
	if (data)
	{
		ft_lstclear(data->lex, free);
		free(data->lex);
		free_final_lex(data->final_lex);
		free_env(data->env);
		if (data->ast)
		{
			free_ast(*data->ast);
			free(data->ast);
		}
		free(data->input);
	}
}

void	data_destroy_exit(t_data *data, int status, char *name, char *err)
{
	data_destroy(data, name, err);
	free(data);
	clear_history();
	exit(status);
}

void	reloop(t_data *data, char *name, char *err)
{
	data_destroy(data, name, err);
	ft_loop(data);
}
