/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 20:57:57 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/23 01:28:54 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_and(t_data *data, t_ast *c, int in, int out)
{
	executer(data, c->left, in, out);
	if (data->exit == 0)
		executer(data, c->right, in, out);
		
	if (c->prev_node == PIPE)
		data_destroy_exit(data, data->exit, NULL, NULL);
		
	//si prev_node == PIPE -> exit
	//si prev_node == start -> rien ou reloop
	//si prev_node == AND_OR -> rien
}

void	exec_or(t_data *data, t_ast *c, int in, int out)
{
	executer(data, c->left, in, out);
	if (data->exit != 0)
		executer(data, c->right, in, out);
	if (c->prev_node == PIPE)
		data_destroy_exit(data, data->exit, NULL, NULL);
}

void	executer(t_data *data, t_ast *c, int in, int out)
{
	if (c->token == AND)
		exec_and(data, c, in, out);
	else if (c->token == OR)
		exec_or(data, c, in, out);
	else if (c->token == PIPE)
		exec_pipe(data, c, in, out);
	else
		exec_expr(data, c, in, out);
}

void	exec(t_data *data)
{
	t_ast	*c;

	c = (*data->ast);
	heredoc(data, c);
	print_ast(*data->ast, 0);
	executer(data, c, STDIN_FILENO, STDOUT_FILENO);
}
