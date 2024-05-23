/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 19:15:16 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/23 15:18:20 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	child_left(t_data *data, t_ast *c, int fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		data_destroy_exit(data, EXIT_FAILURE, "fork", strerror(errno));
	else if (pid == 0)
	{
		// dprintf(2, "child left str = %s | in: %d | out: %d\n", c->str, in, fd[1]);
		// if (data->out != STDOUT_FILENO)
		// 	close(data->out);
		close(fd[0]);
		executer(data, c, data->in, fd[1]);
	}
	return (pid);
}


pid_t	child_right(t_data *data, t_ast *c, int fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		data_destroy_exit(data, EXIT_FAILURE, "fork", strerror(errno));
	else if (pid == 0)
	{
		// dprintf(2, "child right str = %s | in: %d | out: %d\n", c->str, fd[0], out);
		// if (data->in != STDIN_FILENO)
		// 	close(data->in);
		close(fd[1]);
		executer(data, c, fd[0], data->out);
	}
	return (pid);
}

void	close_parent(t_data *data, t_ast *c, int fd[2])
{
	if (data->in != STDIN_FILENO && c->prev_node != AND && c->prev_node != OR)
		close(data->in);
	if (data->out != STDOUT_FILENO && c->prev_node != AND && c->prev_node != OR)
		close(data->out);
	close(fd[0]);
	close(fd[1]);
}

void	exec_pipe(t_data *data, t_ast *c)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	data->pipelvl++;
	if (pipe(fd) == -1)
	{
		if (c->prev_node == AND)
			return ;
		data_destroy_exit(data, EXIT_FAILURE, "pipe", strerror(errno));
	}
	// dprintf(2, "fd[1]: %d | fd[0]: %d\n", fd[1], fd[0]);
	pid1 = child_left(data, c->left, fd);
	pid2 = child_right(data, c->right, fd);
	close_parent(data, c, fd);
	waitpid(pid1, &status, 0);
	if (WIFEXITED(status))
		data->exit = WEXITSTATUS(status);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		data->exit = WEXITSTATUS(status);
	// dprintf(2, "exit: %d\n", data->exit);

	if (c->prev_node == PIPE)
		data_destroy_exit(data, data->exit, NULL, NULL);
}
