/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:09:54 by glemaire          #+#    #+#             */
/*   Updated: 2024/06/01 20:04:48 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cmd_len(t_ast *c)
{
	int	i;

	i = 0;
	while (c)
	{
		if (c->token == WORD)
			i++;
		c = c->right;
	}
	return (i);
}

static char	**cmd_array(t_data *data, t_ast *c)
{
	t_ast	*temp;
	int		i;
	char	**args;

	args = (char **)malloc((cmd_len(c) + 1) * sizeof(char *));
	if (!args)
		data_destroy_exit(data, EXIT_FAILURE, "args", strerror(ENOMEM));
	temp = c;
	i = 0;
	while (temp)
	{
		if (temp->token == WORD)
		{
			args[i] = ft_strdup(temp->str);
			if (!args[i])
				data_destroy_exit(\
				data, EXIT_FAILURE, "args[i]", strerror(ENOMEM));
			i++;
		}
		temp = temp->right;
	}
	args[i] = 0;
	return (args);
}

static char	*getenv_minishell(t_data *data, char *var)
{
	t_list	*c;
	char	*new;
	char	*path;

	new = ft_strjoin(var, "=");
	if (!new)
		data_destroy_exit(data, EXIT_FAILURE, "new", strerror(ENOMEM));
	c = (*data->env);
	path = NULL;
	while (c)
	{
		if (!ft_strncmp(c->content, new, ft_strlen(new)))
		{
			path = ft_substr(c->content, ft_strlen(new), ft_strlen(c->content));
			if (!path)
				data_destroy_exit(data, EXIT_FAILURE, "path", strerror(ENOMEM));
		}
		c = c->next;
	}
	free(new);
	return (path);
}

static char	**get_path(t_data *data, char **args)
{
	char	*bin;
	char	**path;
	int		i;

	bin = getenv_minishell(data, "PATH");
	path = ft_split(bin, ':');
	free(bin);
	if (!path)
		data_destroy_exit(data, EXIT_FAILURE, "path", strerror(ENOMEM));
	i = 0;
	while (path[i])
	{
		path[i] = gnl_strjoin(path[i], "/");
		path[i] = gnl_strjoin(path[i], args[0]);
		i++;
	}
	return (path);
}

void	check_file(t_data *data)
{
	struct stat file_info;
	
	if (ft_strchr(data->args[0], '/'))
	{
		if (lstat(data->args[0], &file_info) == -1)
		{
			if (errno == ENOTDIR)
				data_destroy_exit(data, 126, data->args[0], strerror(errno));
			data_destroy_exit(data, 127, data->args[0], strerror(errno));
		}
		else
		{
			if (S_ISDIR(file_info.st_mode))
				data_destroy_exit(data, 126, data->args[0], strerror(EISDIR));
			if (access(data->args[0], X_OK) == -1)
				data_destroy_exit(data, 126, data->args[0], strerror(errno));
		}
		execve(data->args[0], data->args, data->envp);
		data_destroy_exit(data, EXIT_SUCCESS, NULL, NULL);
	}
}

void	do_execve(t_data *data)
{
	int		i;

	i = 0;
	check_file(data);
	while (data->path[i])
		execve(data->path[i++], data->args, NULL);
	data_destroy_exit(data, 127, data->args[0], CMDNF);
}

void	exec_cmd(t_data *data, t_ast *c)
{
	data->args = cmd_array(data, c);
	// si pas d'argument (only redir) => alors on close tout
	if (data->args[0] == 0)
	{
		if (data->in != STDIN_FILENO)
			close(data->in);
		if (data->out != STDOUT_FILENO)
			close(data->out);
		data_destroy_exit(data, EXIT_SUCCESS, NULL, NULL);
	}
	data->path = get_path(data, data->args);
	do_execve(data);
}
