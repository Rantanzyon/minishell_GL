/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 20:57:57 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/23 11:23:45 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_and(t_data *data, t_ast *c)
{
	// dprintf(2, "node: %s | int: %d | out: %d\n", c->str, data->in, data->out);
	executer(data, c->left, data->in, data->out);
	if (data->exit == 0)
	{
		// dprintf(2, "and\n");
		// dprintf(2, "node: %s | int: %d | out: %d\n", c->str, data->in, data->out);
		executer(data, c->right, data->in, data->out);
	}
		
	if (c->prev_node == PIPE)
	{
		if (data->in != STDIN_FILENO)
			close(data->in);
		if (data->out != STDOUT_FILENO)
			close(data->out);
		data_destroy_exit(data, data->exit, NULL, NULL);
	}
		
	//si prev_node == PIPE -> exit
	//si prev_node == start -> rien ou reloop
	//si prev_node == AND_OR -> rien
}


void	exec_or(t_data *data, t_ast *c)
{
	executer(data, c->left, data->in, data->out);
	dprintf(2, "node: %s | exit: %d | int: %d | out: %d\n", c->str, data->exit, data->in, data->out);
	
	if (data->exit != 0)
		executer(data, c->right, data->in, data->out);
	dprintf(2, "node: %s | exit: %d | int: %d | out: %d\n", c->str, data->exit, data->in, data->out);

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
