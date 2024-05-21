/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_and_or3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 19:17:31 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/20 23:05:37 by glemaire         ###   ########.fr       */
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

void	exec_and(t_data *data, t_ast *c, int in, int out)
{
	//ft_putstr_fd("AND start\n", STDERR_FILENO);
	executer(data, c->left, in, out);
	//ft_putendl_fd(ft_itoa(data->exit), STDERR_FILENO);
	
	if (data->exit == 0)
		executer(data, c->right, in, out);
	else
	{
		while (c && c->token == AND)
			c = c->right;
		if (c && c->token == OR)
			executer(data, c->right, in, out);
	}
	//ft_putstr_fd("END EXEC AND\n", STDERR_FILENO);
}

void	exec_or(t_data *data, t_ast *c, int in, int out)
{
	//ft_putstr_fd("OR start\n", STDERR_FILENO);
	executer(data, c->left, in, out);
	//ft_putendl_fd(ft_itoa(data->exit), STDERR_FILENO);
	if (data->exit != 0)
		executer(data, c->right, in, out);
	else
	{
		while (c && c->token == OR)
			c = c->right;
		if (c && c->token == AND)
			executer(data, c->right, in, out);
	}
	//ft_putstr_fd("END EXEC OR\n", STDERR_FILENO);
}
