/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 19:15:16 by glemaire          #+#    #+#             */
/*   Updated: 2024/06/04 13:51:00 by glemaire         ###   ########.fr       */
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
		//close_out(data);
		//close(fd[0]);
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
		//close_in(data);
		//close(fd[1]);
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

void	print_fds(t_data *data)
{
	t_fds *c;

	dprintf(2, "--------\n");
	// dprintf(2, "in: %d | out: %d\n", data->in, data->out);
	if (!data->fds || !*data->fds)
		return ;
	c = *data->fds;
	while (c)
	{
		if (!c->content)
			dprintf(2, "fd : closed\n");
		else
			dprintf(2, "fd : %d\n", c->content);
		c = c->next;
	}
	dprintf(2, "--------\n");
}


void	close_useless_fds(t_data *data)
{
	t_fds *c;

	if (!data->fds || !*data->fds)
		return ;
	c = *data->fds;
	while (c)
	{
		if (c->content)
		{
			if (data->in != c->content && data->out != c->content)
			{
				close(c->content);
				c->content = 0;
			}
		}
		c = c->next;
	}
}

void	close_all_fds(t_data *data)
{
	t_fds *c;

	if (!data->fds)
		return ;
	c = *data->fds;
	while (c)
	{
		if (c->content)
		{
			close(c->content);
			c->content = 0;
		}
		// dprintf(2, "%d\n", *(int*)c->content);
		c = c->next;
	}
	// dprintf(2, "--------\n");
}


void	close_specific_fd(t_data *data, int fd)
{
	t_fds *c;

	c = *data->fds;
	while (c)
	{
		if (c->content && c->content == fd)
		{
			close(c->content);
			c->content = 0;
		}
		c = c->next;
	}
}

void	add_fd(t_data *data, int fd)
{
	t_fds	*new;
	t_fds	*c;

	new = malloc(sizeof(t_fds));
	if (!new)
		return ; // data destroy
	new->content = fd;
	new->next = NULL;

	c = *data->fds;
	if (c)
	{
		while (c->next)
			c = c->next;
		c->next = new;
	}
	else
		*data->fds = new;
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
	add_fd(data, fd[0]);
	add_fd(data, fd[1]);
	// print_fds(data);
	pid1 = child_left(data, c->left, fd);
	pid2 = child_right(data, c->right, fd);
	close_specific_fd(data, fd[0]);
	close_specific_fd(data, fd[1]);
	// print_fds(data);
	// dprintf(2, "node: %s | int: %d | out: %d\n", c->str, data->in, data->out);
	// close_useless_fds(data, data->fds);
	waitpid(pid1, &status, 0);
	if (WIFEXITED(status))
		data->exit = WEXITSTATUS(status);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		data->exit = WEXITSTATUS(status);
	if (c->prev && c->prev->token == PIPE)
		data_destroy_exit(data, data->exit, NULL, NULL);
	// dprintf(2, "pwet\n");
}
