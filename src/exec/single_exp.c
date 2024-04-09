/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_exp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 22:28:43 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/09 19:28:17 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	single_expr(t_data *data, t_ast *c)
{
	pid_t	pid;
	int	status;

	//check_exit(data, c);
	pid = fork();
	if (pid == -1)
		reloop(data, "fork()");
	if (pid == 0)
	{
		update_redir(data, c);
		dup2(data->fd_in, STDIN_FILENO);
		dup2(data->fd_out, STDOUT_FILENO);
		check_builtin(data, c);
		exec_cmd(data, c);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit = WEXITSTATUS(status);
	reloop(data, NULL);
}
