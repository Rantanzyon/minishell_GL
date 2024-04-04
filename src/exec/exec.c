/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 20:57:57 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/04 09:14:19 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	l_redir(t_data *data, t_ast *c)
{
	if (data->exec->fd_in != STDIN_FILENO)
		close(data->exec->fd_in);
	data->exec->fd_in = open(c->str, O_RDONLY);
	if (data->exec->fd_in == -1)
		data_destroy_exit(data, 1, strerror(errno));
}

void	ll_redir(t_data *data)
{
	if (data->exec->fd_in != STDIN_FILENO)
		close(data->exec->fd_in);
	data->exec->fd_in = data->exec->here_doc;
}

void	r_redir(t_data *data, t_ast *c)
{
	if (data->exec->fd_out != STDIN_FILENO)
		close(data->exec->fd_out);
	data->exec->fd_out = open(c->str,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->exec->fd_out == -1)
		data_destroy_exit(data, 1, strerror(errno));
}

void	rr_redir(t_data *data, t_ast *c)
{
	if (data->exec->fd_out != STDIN_FILENO)
		close(data->exec->fd_out);
	data->exec->fd_out = open(c->str,
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (data->exec->fd_out == -1)
		data_destroy_exit(data, 1, strerror(errno));
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

	//args
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
		data_destroy_exit(data, 1, "args: Malloc error");
	temp = c;
	i = 0;
	while (temp)
	{
		if (temp->token == WORD)
		{
			args[i] = ft_strdup(temp->str);
			if (!args[i])
				data_destroy_exit(data, 1, "args[i]: Malloc error");
			i++;
		}
		temp = temp->right;
	}
	args[i] = 0;



	//path
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
				data_destroy_exit(data, 1, "bin: Malloc error");
			break ;
		}
		cur = cur->next;
	}
	path = ft_split(bin, ':');
	if (!path)
		data_destroy_exit(data, 1, "path: Malloc error");

	i = 0;
	while (path[i])
	{
		path[i] = gnl_strjoin(path[i], "/");
		path[i] = gnl_strjoin(path[i], args[0]);
		//printf("%s\n", path[i]);
		i++;
	}

	int		res;

	i = 0;
	res = 0;
	while (path[i])
	{
		res = execve(path[i], args, NULL);
		//free(path);
		i++;
	}
	if (res == -1)
		data_destroy_exit(data, 1, "cmd doesnt exist");
}

void	exec(t_data *data, t_ast *c, int in, int out)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int	status;

	if (c->token == PIPE)
	{
		if (pipe(fd) == -1)
			data_destroy_exit(data, -1, "pipe fail");

		printf("fd[0] = %d | fd[1] = %d\n", fd[0], fd[1]);


		
		pid2 = fork();
		if (pid2 < 0)
			data_destroy_exit(data, EXIT_FAILURE, strerror(errno));
		else if (pid2 == 0)
		{
			close(fd[0]);
			exec(data, c->left, in, fd[1]);
		}

		pid1 = fork();
		if (pid1 < 0)
			data_destroy_exit(data, EXIT_FAILURE, strerror(errno));
		else if (pid1 == 0)
		{
			close(fd[1]);
			exec(data, c->right, fd[0], out);
		}

		close(fd[0]);
		close(fd[1]);
			
	}
	else
	{
		//child1(data, c, fd_prev);
		data->exec->fd_in = in;
		data->exec->fd_out = out;
		update_redir(data, c);
		
		pid_t pid3;

		printf("str = %s | in = %d | out = %d\n", c->str, data->exec->fd_in, data->exec->fd_out);
		pid3 = fork();
		if (pid3 == 0)
		{
			dup2(data->exec->fd_in, STDIN_FILENO);
			dup2(data->exec->fd_out, STDOUT_FILENO);
			exec_cmd(data, c);
			
		}
		waitpid(pid3, &status, 0);
		if (WIFEXITED(status))
			data->exec->exit_status = WEXITSTATUS(status);
		//reloop(data, NULL);
		
	}
}
