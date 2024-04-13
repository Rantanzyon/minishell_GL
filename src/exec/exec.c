/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 20:57:57 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/14 00:19:50 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_execve(t_data *data, char **path, char **args)
{
	int		res;
	int		i;

	i = 0;
	res = 0;
	while (path[i])
	{
		res = execve(path[i], args, NULL);
		i++;
	}
	if (res == -1)
	{
		//ft_putstr_fd("minishell: ", STDERR_FILENO);
		//ft_putstr_fd(args[0], STDERR_FILENO);
		//ft_putstr_fd(": commanddd not found\n", STDERR_FILENO);
		//err_message(data, args[0], CMDNF);
		
		data_destroy_exit(data, CMD_NF, args[0], CMDNF);
	}
}

void	executer(t_data *data)
{
	t_ast	*c;

	c = (*data->ast);
	heredoc(data, c);
	if (c->token == PIPE)
		multi_expr(data, c, STDIN_FILENO, STDOUT_FILENO);
	else
		single_expr(data, c);
}
