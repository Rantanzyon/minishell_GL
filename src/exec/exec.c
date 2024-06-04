/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 20:57:57 by glemaire          #+#    #+#             */
/*   Updated: 2024/06/04 10:17:08 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_and(t_data *data, t_ast *c, int in, int out)
{
	//data->in = in;
	//data->out = out;
	executer(data, c->left, in, out);
	// print_fds(data);
	// dprintf(2, "node: %s | int: %d | out: %d\n", c->str, in, out);
	//data->in = in;
	//data->out = out;
	if (data->exit == 0)
		executer(data, c->right, in, out);
	if (c->prev && c->prev->token == PIPE)
		data_destroy_exit(data, data->exit, NULL, NULL);
}

void	exec_or(t_data *data, t_ast *c, int in, int out)
{
	//data->in = in;
	//data->out = out;
	executer(data, c->left, in, out);
	// print_fds(data);
	//data->in = in;
	//data->out = out;
	if (data->exit != 0)
		executer(data, c->right, in, out);
	if (c->prev && c->prev->token == PIPE)
		data_destroy_exit(data, data->exit, NULL, NULL);
}

void	executer(t_data *data, t_ast *c, int in, int out)
{
	// dprintf(2, "node: %s | int: %d | out: %d\n", c->str, in, out);
	data->in = in;
	data->out = out;
	if (c->token == AND)
		exec_and(data, c, in, out);
	else if (c->token == OR)
		exec_or(data, c, in, out);
	else if (c->token == PIPE)
		exec_pipe(data, c);
	else
		exec_expr(data, c, in, out);
	//dprintf(2, "pwet\n");
}


void	exec(t_data *data)
{
	t_ast	*c;

	c = (*data->ast);
	heredoc(data, c);
	print_ast(*data->ast, 0);
	data->fds = malloc(sizeof(t_list *));
	if (!data->fds)
		reloop(data, EXIT_FAILURE, "data->fds", strerror(ENOMEM));
	*data->fds = NULL;
	executer(data, c, STDIN_FILENO, STDOUT_FILENO);
}
