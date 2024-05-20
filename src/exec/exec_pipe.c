/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 19:15:16 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/20 00:20:15 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	child_left(t_data *data, t_ast *c, int fd[2], int in)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		data_destroy_exit(data, EXIT_FAILURE, "fork", strerror(errno));
	else if (pid == 0)
	{
		close(fd[0]);
		executer(data, c, in, fd[1]);
	}
	return (pid);
}


pid_t	child_right(t_data *data, t_ast *c, int fd[2], int out)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		data_destroy_exit(data, EXIT_FAILURE, "fork", strerror(errno));
	else if (pid == 0)
	{
		close(fd[1]);
		executer(data, c, fd[0], out);
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

void	exec_pipe(t_data *data, t_ast *c, int in, int out)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	c->pipe_lvl++;
	if (pipe(fd) == -1)
		data_destroy_exit(data, EXIT_FAILURE, "pipe", strerror(errno));
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
		data_destroy_exit(data, data->exit, NULL, NULL);
	else
		reloop(data, NULL, NULL);
}
