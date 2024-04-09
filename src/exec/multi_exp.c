/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 22:29:11 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/09 19:28:54 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expression(t_data *data, t_ast *c, int in, int out)
{
	data->fd_in = in;
	data->fd_out = out;
	update_redir(data, c);
	dup2(data->fd_in, STDIN_FILENO);
	dup2(data->fd_out, STDOUT_FILENO);
	//check_exit(data, c);
	check_builtin(data, c);
	exec_cmd(data, c);
}


pid_t	child_left(t_data *data, t_ast *c, int fd[2], int in)
{	
	pid_t	pid;
	
	pid = fork();
 	if (pid == -1)
		data_destroy_exit(data, EXIT_FAILURE, "fork()");
	else if (pid == 0)
	{
		close(fd[0]);
		multi_expr(data, c, in, fd[1]);
	}
	return (pid);
}

pid_t	child_right(t_data *data, t_ast *c, int fd[2], int out)
{	
	pid_t	pid;
	
	pid = fork();
 	if (pid == -1)
		data_destroy_exit(data, EXIT_FAILURE, "fork()");
	else if (pid == 0)
	{
		close(fd[1]);
		multi_expr(data, c, fd[0], out);
	}
	return (pid);
}

void	close_parent(int fd[2], int in, int out)
{
	if (in != STDIN_FILENO)
		close(in);
	if (out != STDOUT_FILENO)
		close(out);
	close(fd[0]);
	close(fd[1]);
}

void	multi_expr(t_data *data, t_ast *c, int in, int out)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	if (c->token == PIPE)
	{
		if (pipe(fd) == -1)
			data_destroy_exit(data, EXIT_FAILURE, "pipe()");
		pid1 = child_left(data, c->left, fd, in);
		pid2 = child_right(data, c->right, fd, out);
		close_parent(fd, in, out);
		waitpid(pid1, &status, 0);
		if (WIFEXITED(status))
			data->exit = WEXITSTATUS(status);
		waitpid(pid2, &status, 0);
		if (WIFEXITED(status))
			data->exit = WEXITSTATUS(status);
 		if (c != *(data->ast))
			data_destroy_exit(data, data->exit, NULL);
		else
			reloop(data, NULL);
	}
	else
		expression(data, c, in, out);
}
