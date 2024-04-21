/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_exp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 22:28:43 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/20 22:32:29 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	single_expr(t_data *data, t_ast *c, int in, int out)
{
	pid_t	pid;
	int	status;

	//check_exit(data, c);
	pid = fork();
	if (pid == -1)
		reloop(data, "fork", strerror(errno));
	if (pid == 0)
	{
		data->fd_in = in;
		data->fd_out = out;
		update_redir(data, c);
		dup2(data->fd_in, STDIN_FILENO);
		dup2(data->fd_out, STDOUT_FILENO);
		check_builtin(data, c);
		exec_cmd(data, c);
		
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit = WEXITSTATUS(status);
	reloop(data, NULL, NULL);
}
