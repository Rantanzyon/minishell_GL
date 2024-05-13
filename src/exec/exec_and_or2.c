/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_and_or2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 19:17:31 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/13 17:04:20 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_and_or(t_data *data, t_ast *c, int in, int out)
{
	pid_t	pid1;
	int		status;

	//ft_putendl_fd("child", STDERR_FILENO);
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

void	special_builtin(t_data *data, t_ast *c, int in, int out)
{
	data->fd_in = in;
	data->fd_out = out;
	update_redir(data, c);
	dup2(data->fd_in, STDIN_FILENO);
	dup2(data->fd_out, STDOUT_FILENO);
	//ft_putendl_fd("special", STDERR_FILENO);
	if (c->exit_pipe == BUILTIN_EXIT)
		builtin_exit(data, c);
	else if (c->exit_pipe == BUILTIN_CD)
	{
		
	}
	
}

void	exec_and_or(t_data *data, t_ast *c, int in, int out)
{
	if (c->exit_pipe == FIRST_ROW && c->left->exit_pipe > 0)
		special_builtin(data, c->left, STDIN_FILENO, STDOUT_FILENO);
	else
		child_and_or(data, c->left, in, out);


	
	if ((data->exit == 0 && c->token == AND) || \
		(data->exit != 0 && c->token == OR))
		{
		//ft_putendl_fd("test", STDERR_FILENO);	
		executer(data, c->right, in, out);
		}
	else
	{
		if (c->token == OR)
		{
			while (c && c->token != AND)
				c = c->right;
		}
		else if (c->token == AND)
		{
			while (c && c->token != OR)
				c = c->right;
		}
		if (c)
			executer(data, c->right, in, out);
		else
			//ft_putendl_fd("wtf", STDERR_FILENO);
			reloop(data, "reloop", NULL);
	}
/* 	if (c != *(data->ast))
		data_destroy_exit(data, data->exit, "destroy", NULL);
	else
		reloop(data, "reloop", NULL); */
}
