/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 19:15:16 by glemaire          #+#    #+#             */
/*   Updated: 2024/06/03 02:26:15 by glemaire         ###   ########.fr       */
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
		close_out(data);
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
		close_in(data);
		close(fd[1]);
		executer(data, c, fd[0], data->out);
	}
	return (pid);
}

void	close_parent(t_data *data, t_ast *c, int fd[2])
{
	// dprintf(2, "%p\n", c->prev);
	if (!c->prev || c->prev->token == PIPE)
	{
		if (data->in != STDIN_FILENO)
			close(data->in);
		if (data->out != STDOUT_FILENO)
			close(data->out);	
	}
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
		data_destroy_exit(data, EXIT_FAILURE, "pipe", strerror(errno));
	pid1 = child_left(data, c->left, fd);
	pid2 = child_right(data, c->right, fd);
	close_parent(data, c, fd);
	waitpid(pid1, &status, 0);
	if (WIFEXITED(status))
		data->exit = WEXITSTATUS(status);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		data->exit = WEXITSTATUS(status);
	if (c->prev && c->prev->token == PIPE)
		data_destroy_exit(data, data->exit, NULL, NULL);
}
