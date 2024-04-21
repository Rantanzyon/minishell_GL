/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 20:57:57 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/21 17:58:36 by glemaire         ###   ########.fr       */
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
		res = execve(path[i++], args, NULL);
	if (res == -1)
		data_destroy_exit(data, CMD_NF, args[0], CMDNF);
}

void	executer(t_data *data, t_ast *c, int in, int out)
{
	if (c->token == PIPE)
		exec_pipe(data, c, in, out);
	else if (c->token == AND || c->token == OR)
		exec_and_or(data, c, in, out);
	else
		exec_expr(data, c, in, out);
}

void	exec(t_data *data)
{
	t_ast	*c;

	c = (*data->ast);
	heredoc(data, c);
	executer(data, c, STDIN_FILENO, STDOUT_FILENO);
}
