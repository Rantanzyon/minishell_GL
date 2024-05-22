/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 19:16:43 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/23 01:24:35 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_expr(t_data *data, t_ast *c, int in, int out)
{
	pid_t	pid;
	int		status;
	
	
	check_builtin(data, c, in, out);
	pid = fork();
	if (pid == -1)
		reloop(data, "fork", strerror(errno));
	if (pid == 0)
	{
		update_redir(data, c, in, out);
		exec_cmd(data, c, in, out);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit = WEXITSTATUS(status);

	if (c->prev_node == PIPE)
		data_destroy_exit(data, data->exit, NULL, NULL);
}

