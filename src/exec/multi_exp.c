/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 22:29:11 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/21 00:21:38 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expression(t_data *data, t_ast *c, int in, int out)
{
	//printf("test = %s\n", c->str);
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
	
	//printf("child left: %s\n", c->str);
	pid = fork();
 	if (pid == -1)
		data_destroy_exit(data, EXIT_FAILURE, "fork", strerror(errno));
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
		data_destroy_exit(data, EXIT_FAILURE, "fork", strerror(errno));
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

int	prev_node(t_ast *a, t_ast *c, int i)
{
	
	//ft_putstr_fd(ft_itoa(i), STDERR_FILENO);
	if (a->right == c)
	{
		if (a->token != PIPE)
			return (1);
		else
			return (0);
	}

	if (a->right)
		i = prev_node(a->right, c, 0);
/* 	if (a->left)
		i = prev_node(a->left, c, 0); */
	return (i);
	
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
		//printf("%d\n", prev_node(*data->ast, *data->ast, c));
		//ft_putendl_fd("NIQUE", STDERR_FILENO);
 		//if (!prev_node(*data->ast, c, 0) && c != *(data->ast))
 		if (c != *(data->ast))
			data_destroy_exit(data, data->exit, NULL, NULL);
		else
			reloop(data, NULL, NULL);
	}
	else if (c->token == AND || c->token == OR)
	{
		//if (pipe(fd) == -1)
		//	data_destroy_exit(data, EXIT_FAILURE, "pipe", strerror(errno));
		pid1 = fork();
		if (pid1 == -1)
			data_destroy_exit(data, EXIT_FAILURE, "fork", strerror(errno));
		else if (pid1 == 0)
			multi_expr(data, c->left, in, out);
		waitpid(pid1, &status, 0);
		if (WIFEXITED(status))
			data->exit = WEXITSTATUS(status);
		else
			data->exit = 0;
		//printf("PROUT\n");
		if ((data->exit == 0 && c->token == AND) || (data->exit != 0 && c->token == OR))
		{
			pid2 = fork();
			if (pid2 == -1)
				data_destroy_exit(data, EXIT_FAILURE, "fork", strerror(errno));
			else if (pid2 == 0)
				multi_expr(data, c->right, in, out);
			waitpid(pid2, &status, 0);
			if (WIFEXITED(status))
				data->exit = WEXITSTATUS(status);
			else
				data->exit = 0;
		}
		if (c != *(data->ast))
			data_destroy_exit(data, data->exit, "destroy", NULL);
		else
			reloop(data, "reloop", NULL);	
	}
	else if (c->token == PAR_L)
		multi_expr(data, c->left, in, out);
	else
		expression(data, c, in, out);
}
/*
((((echo start || ls) | >out1 cat >out2) && echo bonjour || echo salut) |<<LIM grep a) && (wsdzx || sleep 3) && <<END cat
*/