/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:09:54 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/20 02:32:07 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		cmd_len(t_ast *c)
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

char	**cmd_array(t_data *data, t_ast *c)
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
				data_destroy_exit(data, EXIT_FAILURE, "args[i]", strerror(ENOMEM));
			i++;
		}
		temp = temp->right;
	}
	args[i] = 0;
	return (args);
}

char	*getenv_minishell(t_data *data, char *var)
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
	return (path);
}

char	**get_path(t_data *data, char **args)
{
	char	*bin;
	char	**path;
	int		i;

	bin = getenv_minishell(data, "PATH");
	path = ft_split(bin, ':');
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

void	exec_cmd(t_data *data, t_ast *c)
{
	char	**args;
	char	**path;

	args = cmd_array(data, c);
	path = get_path(data, args);
	do_execve(data, path, args);
}
