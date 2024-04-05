/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 20:57:57 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/05 12:47:59 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	l_redir(t_data *data, t_ast *c)
{
	if (data->fd_in != STDIN_FILENO)
		close(data->fd_in);
	data->fd_in = open(c->str, O_RDONLY);
	if (data->fd_in == -1)
		data_destroy_exit(data, EXIT_FAILURE, c->str);
}

void	ll_redir(t_data *data)
{
	if (data->fd_in != STDIN_FILENO)
		close(data->fd_in);
	data->fd_in = data->here_doc;
}

void	r_redir(t_data *data, t_ast *c)
{
	if (data->fd_out != STDIN_FILENO)
		close(data->fd_out);
	data->fd_out = open(c->str,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->fd_in == -1)
		data_destroy_exit(data, EXIT_FAILURE, c->str);
}

void	rr_redir(t_data *data, t_ast *c)
{
	if (data->fd_out != STDIN_FILENO)
		close(data->fd_out);
	data->fd_out = open(c->str,
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (data->fd_out == -1)
	if (data->fd_in == -1)
		data_destroy_exit(data, EXIT_FAILURE, c->str);
}

void	update_redir(t_data *data, t_ast *c)
{
	int	token;

	token = c->token;
	while (token != PIPE && token != WORD && token != FILENAME)
	{
		if (token == L_REDIR)
			l_redir(data, c->left);
		else if (token == LL_REDIR)
			ll_redir(data);
		else if (token == R_REDIR)
			r_redir(data, c->left);
		else
			rr_redir(data, c->left);
		c = c->right;
		token = c->token;
	}
}

void	exec_cmd(t_data *data, t_ast *c)
{
	t_ast	*temp;
	int		i;
	char	**args;

	temp = c;
	i = 0;
	while (temp)
	{
		if (temp->token == WORD)
			i++;
		temp = temp->right;
	}
	args = (char **)malloc((i + 1) * sizeof(char *));
	if (!args)
		data_destroy_exit(data, EXIT_FAILURE, "args");
	temp = c;
	i = 0;
	while (temp)
	{
		if (temp->token == WORD)
		{
			args[i] = ft_strdup(temp->str);
			if (!args[i])
				data_destroy_exit(data, EXIT_FAILURE, "args[i]");
			i++;
		}
		temp = temp->right;
	}
	args[i] = 0;



	t_list	*cur;
	char	*bin;
	char	**path;

	cur = (*data->env);
	while (cur)
	{
		if (!ft_strncmp(cur->content, "PATH=", 4))
		{
			bin = ft_substr(cur->content, 5, ft_strlen(cur->content));
			if (!bin)
				data_destroy_exit(data, EXIT_FAILURE, "bin");
			break ;
		}
		cur = cur->next;
	}
	path = ft_split(bin, ':');
	if (!path)
		data_destroy_exit(data, EXIT_FAILURE, "path");

	i = 0;
	while (path[i])
	{
		path[i] = gnl_strjoin(path[i], "/");
		path[i] = gnl_strjoin(path[i], args[0]);
		i++;
	}

	int		res;

	i = 0;
	res = 0;
	while (path[i])
	{
		res = execve(path[i], args, NULL);
		i++;
	}
	if (res == -1)
			data_destroy_exit(data, 1, args[0]);
}

void	expression(t_data *data, t_ast *c, int in, int out)
{
	data->fd_in = in;
	data->fd_out = out;
	update_redir(data, c);
	dup2(data->fd_in, STDIN_FILENO);
	dup2(data->fd_out, STDOUT_FILENO);
	exec_cmd(data, c);
}

void	single_expr(t_data *data, t_ast *c)
{
	pid_t	pid;
	int	status;

	pid = fork();
	if (pid == -1)
		reloop(data, "fork()");
	if (pid == 0)
	{
		update_redir(data, c);
		dup2(data->fd_in, STDIN_FILENO);
		dup2(data->fd_out, STDOUT_FILENO);
		exec_cmd(data, c);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit = WEXITSTATUS(status);
	reloop(data, NULL);
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

void	executer(t_data *data)
{
	t_ast	*c;

	c = (*data->ast);
	//do_heredoc(data, *(data->ast));
	if (c->token == PIPE)
		multi_expr(data, c, STDIN_FILENO, STDOUT_FILENO);
	else
		single_expr(data, c);
}
