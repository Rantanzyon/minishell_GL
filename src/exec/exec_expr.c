/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 19:16:43 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/26 21:02:01 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cmd_fork(t_data *data, t_ast *c)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		reloop(data, EXIT_FAILURE, "fork", strerror(errno));
	if (pid == 0)
	{
		update_redir(data, c);
		exec_cmd(data, c);
	}
	if (data->in != STDIN_FILENO && c->prev_node != AND && c->prev_node != OR)
		close(data->in);
	if (data->out != STDOUT_FILENO && c->prev_node != AND && c->prev_node != OR)
		close(data->out);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit = WEXITSTATUS(status);
}

char	*do_expand(t_data *data, char *str, int *i)
{
	char	*name;
	char	*expand;

	expand = ft_strdup("");
	if (!expand)
		return (NULL);
	*i++;
	if (str[*i] == '\'' || str[*i] == '\"')
		return (expand);
	if (str[*i] != '_' && !ft_isalpha(str[*i]))
	{
		*i++;
		return (expand);
	}
	expand = gnl_strjoin(expand, str[*i]);
	while (str[*i] && )
	{
		
	}
}

void	change_node(t_data *data, t_ast *c)
{
	int	i;
	char	*str;
	char	*temp;

	i = 0;
	str = ft_strdup("");
	if (!str)
		data_destroy_exit(data, EXIT_FAILURE, "str", strerror(ENOMEM));
	while (c->str[i])
	{
		if (c->str[i] == '\'')
		{
			i++;
			while (c->str[i] && c->str[i] != '\'')
			{
				temp = malloc(2);
				temp[0] = c->str[i];
				temp[1] = '\0';
				str = gnl_strjoin(str, temp);
				free(temp);
				i++;
			}
		}
		else if (c->str[i] == '\"')
		{
			i++;
			while (c->str[i] && c->str[i] != '\"')
			{
				if(c->str[i] == '$')
					temp = do_expand(data, c->str, &i);	
				else
				{
					temp = malloc(2);
					temp[0] = c->str[i];
					temp[1] = '\0';
				}
				str = gnl_strjoin(str, temp);
				free(temp);
				i++;
			}
			
		}
		else if(c->str[i] == '$' && c->str[i + 1] && c->str[i + 1] == '?')
		{
			temp = ft_itoa(data->exit);
			str = gnl_strjoin(str, temp);
			free(temp);
			i++;
		}
		else
		{
			temp = malloc(2);
			temp[0] = c->str[i];
			temp[1] = '\0';
			str = gnl_strjoin(str, temp);
			free(temp);
		}
		i++;
	}
	//free(c->str);
	c->str = str;
}

void	change_word(t_data *data, t_ast *c)
{
	if (c->token == WORD || c->token == FILENAME)
		change_node(data, c);
	if (c->right)
		change_word(data, c->right);
	if (c->left)
		change_word(data, c->left);
}

void	exec_expr(t_data *data, t_ast *c)
{
	int	in_backup;
	int	out_backup;
	change_word(data, c);
	print_ast(*data->ast, 0);
	if (is_builtin(c))
	{
		in_backup = dup(data->in);
		out_backup = dup(data->out);
		update_redir(data, c);
		builtin(data, c);
		close_in(data);
		close_out(data);
		data->in = dup(in_backup);
		data->out = dup(out_backup);
	}
	else if (c->prev_node == PIPE)
	{
		update_redir(data, c);
		exec_cmd(data, c);
	}
	else
		exec_cmd_fork(data, c);
}

