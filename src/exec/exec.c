/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 20:57:57 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/30 06:43:17 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_and(t_data *data, t_ast *c)
{
	int	in_backup;
	int	out_backup;
	
	in_backup = dup(data->in);
	out_backup = dup(data->out);
	executer(data, c->left, data->in, data->out);
	close_in(data);
	close_out(data);
	data->in = dup(in_backup);
	data->out = dup(out_backup);
	if (data->exit == 0)
		executer(data, c->right, data->in, data->out);
	if (c->prev_node == PIPE)
	{
		if (data->in != STDIN_FILENO)
			close(data->in);
		if (data->out != STDOUT_FILENO)
			close(data->out);
		data_destroy_exit(data, data->exit, NULL, NULL);
	}
}


void	exec_or(t_data *data, t_ast *c)
{
	int	in_backup;
	int	out_backup;
	
	in_backup = dup(data->in);
	out_backup = dup(data->out);
	executer(data, c->left, data->in, data->out);
	close_in(data);
	close_out(data);
	data->in = dup(in_backup);
	data->out = dup(out_backup);
	if (data->exit != 0)
		executer(data, c->right, data->in, data->out);
	if (c->prev_node == PIPE)
	{
		if (data->in != STDIN_FILENO)
			close(data->in);
		if (data->out != STDOUT_FILENO)
			close(data->out);
		data_destroy_exit(data, data->exit, NULL, NULL);
	}
}

void	executer(t_data *data, t_ast *c, int in, int out)
{
	//dprintf(2, "node: %s | int: %d | out: %d\n", c->str, in, out);
	data->in = in;
	data->out = out;
	if (c->token == AND)
		exec_and(data, c);
	else if (c->token == OR)
		exec_or(data, c);
	else if (c->token == PIPE)
		exec_pipe(data, c);
	else
		exec_expr(data, c);
}

void	exec(t_data *data)
{
	t_ast	*c;

	c = (*data->ast);
	heredoc(data, c);
	print_ast(*data->ast, 0);
	executer(data, c, STDIN_FILENO, STDOUT_FILENO);
}
