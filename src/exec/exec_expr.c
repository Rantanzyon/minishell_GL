/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 19:16:43 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/22 00:32:28 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_exec(t_data *data, t_ast *c, int in, int out)
{
	data->fd_in = in;
	data->fd_out = out;
	update_redir(data, c);
	dup2(data->fd_in, STDIN_FILENO);
	dup2(data->fd_out, STDOUT_FILENO);
	check_builtin(data, c);
	exec_cmd(data, c);
}

void	exec_expr(t_data *data, t_ast *c, int in, int out)
{
	pid_t	pid;
	int		status;
	
	if (c == *data->ast)
	{
		// si exit se trouve au niveau 1, on ne fork pas et data_destroy_exit
		pid = fork();
		if (pid == -1)
			reloop(data, "fork", strerror(errno));
		if (pid == 0)
			setup_exec(data, c, in, out);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			data->exit = WEXITSTATUS(status);
		reloop(data, NULL, NULL);
	}
	else
		setup_exec(data, c, in, out);
}
