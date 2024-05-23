/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 19:16:43 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/23 12:37:01 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cmd_fork(t_data *data, t_ast *c)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		reloop(data, "fork", strerror(errno));
	if (pid == 0)
	{
		update_redir(data, c);
		exec_cmd(data, c);
	}
	if (data->in != STDIN_FILENO && c->prev_node != AND && c->prev_node != OR)
		close(data->in);
	if (data->out != STDOUT_FILENO && c->prev_node != AND && c->prev_node != OR)
		close(data->out);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit = WEXITSTATUS(status);
}


void	exec_expr(t_data *data, t_ast *c)
{
	if (is_builtin(c))
		builtin(data, c);
	else if (c->prev_node == PIPE)
	{
		update_redir(data, c);
		exec_cmd(data, c);
	}
	else
		exec_cmd_fork(data, c);
}

