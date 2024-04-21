/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_and_or.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 19:17:31 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/21 19:17:33 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_and_or(t_data *data, t_ast *c, int in, int out)
{
	pid_t	pid1;
	int		status;

	pid1 = fork();
	if (pid1 == -1)
		data_destroy_exit(data, EXIT_FAILURE, "fork", strerror(errno));
	else if (pid1 == 0)
		executer(data, c, in, out);
	waitpid(pid1, &status, 0);
	if (WIFEXITED(status))
		data->exit = WEXITSTATUS(status);
	else
		data->exit = 0;
}

void	exec_and_or(t_data *data, t_ast *c, int in, int out)
{
	child_and_or(data, c->left, in, out);
	if ((data->exit == 0 && c->token == AND) || \
		(data->exit != 0 && c->token == OR))
		child_and_or(data, c->right, in, out);
	if (c != *(data->ast))
		data_destroy_exit(data, data->exit, "destroy", NULL);
	else
		reloop(data, "reloop", NULL);
}
