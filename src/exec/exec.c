/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 20:57:57 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/20 12:40:19 by glemaire         ###   ########.fr       */
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
	//ft_putendl_fd("executer", STDERR_FILENO);
	if (c->token == AND)
		exec_and(data, c, in, out);
	else if (c->token == OR)
		exec_or(data, c, in, out);
	else if (c->token == PIPE)
		exec_pipe(data, c, in, out);
	else
		exec_expr(data, c, in, out);
}

void	is_builtin_ast(t_ast *c)
{
	t_ast *temp;

	temp = c;
	while (c && c->token != WORD)
		c = c->right;
	if (!c)
		return ;
	else if (!ft_strcmp(c->str, "exit"))
		temp->exit_pipe = BUILTIN_EXIT;
	else if (!ft_strcmp(c->str, "cd"))
		temp->exit_pipe = BUILTIN_CD;
}

void	t2(t_ast *c)
{
	if (c->exit_pipe == 0)
	{
		if (c->left && (c->left->token != OR && c->left->token != AND && c->left->token != PIPE))
			is_builtin_ast(c->left);
		if (c->right && (c->right->token != OR && c->right->token != AND && c->right->token != PIPE))
			is_builtin_ast(c->right);
	}
	if (c->right)
		t2(c->right);
	if (c->left)
		t2(c->left);
}
void	t1(t_ast *c)
{
	if (c->token == AND || c->token == OR)
		c->exit_pipe = FIRST_ROW;
	if (c->right)
		t1(c->right);
}

void	t3(t_ast *c)
{
	if (c->token != AND && c->token != OR && c->token != PIPE)
		is_builtin_ast(c);
}

void	exec(t_data *data)
{
	t_ast	*c;

	c = (*data->ast);
	heredoc(data, c);
	t1(*data->ast);
	t2(*data->ast);
	t3(*data->ast);
	print_ast(*data->ast, 0);
	executer(data, c, STDIN_FILENO, STDOUT_FILENO);
}
