/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 19:16:43 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/23 04:26:43 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_expr(t_data *data, t_ast *c, int in, int out)
{
	pid_t	pid;
	int		status;
	
	data->in = in;
	data->out = out;
	
	check_builtin(data, c);
	pid = fork();
	if (pid == -1)
		reloop(data, "fork", strerror(errno));
	if (pid == 0)
	{
		update_redir(data, c);
		exec_cmd(data, c);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit = WEXITSTATUS(status);

	if (c->prev_node == PIPE)
		data_destroy_exit(data, data->exit, NULL, NULL);
}

