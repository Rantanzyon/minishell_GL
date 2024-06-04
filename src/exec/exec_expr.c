/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_expr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 19:16:43 by glemaire          #+#    #+#             */
/*   Updated: 2024/06/04 13:45:52 by glemaire         ###   ########.fr       */
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
		// dprintf(2, "node: %s | int: %d | out: %d\n", c->str, data->in, data->out);
		update_redir(data, c);
		// dprintf(2, "node: %s | int: %d | out: %d\n", c->str, data->in, data->out);
		// print_fds(data);
		close_useless_fds(data);
		// print_fds(data);
		dup2(data->in, STDIN_FILENO);
		dup2(data->out, STDOUT_FILENO);
		exec_cmd(data, c);
	}
/* 	if (data->in != STDIN_FILENO && c->prev_node != AND && c->prev_node != OR)
		close(data->in);
	if (data->out != STDOUT_FILENO && c->prev_node != AND && c->prev_node != OR)
		close(data->out); */
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit = WEXITSTATUS(status);
}


// static void	ft_addnode(t_data *data, t_list **lst, char c, int pretok)
// {
// 	t_lex	*cursor;
// 	t_list	*node;

// 	cursor = (t_lex *)malloc(sizeof(t_lex));
// 	if (!cursor)
// 		reloop(data, EXIT_FAILURE, "cursor", strerror(ENOMEM));
// 	cursor->c = c;
// 	cursor->pretok = pretok;
// 	node = ft_lstnew(cursor);
// 	if (!node)
// 		reloop(data, EXIT_FAILURE, "node", strerror(ENOMEM));
// 	ft_lstadd_back(lst, node);
// }

int	check_char(char c)
{
	if (c == '"')
		return (DQ);
	else if (c == '\'')
		return (SQ);
	else if (c == '$')
		return (EXP);
	else
		return (CHAR);
}

char	*do_expand(t_data *data, char *str, int *i)
{
	char	*name;
	char	*temp;
	char	*expand;
	int		j;

	expand = ft_strdup("");
	if (!expand)
		return (NULL);
	(*i)++;
	if (!str[*i])
		return (expand);
	if (str[*i] != '_' && !ft_isalpha(str[*i]))
	{
		(*i)++;
		return (expand);
	}
	name = ft_strdup("");
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
	{
		temp = malloc(2);
		temp[0] = str[*i];
		temp[1] = '\0';
		name = gnl_strjoin(name, temp);
		(*i)++;
	}

	j = 0;
	while (data->envp[j])
	{
		if (!strncmp(name, data->envp[j], ft_strlen(name)) && data->envp[j][ft_strlen(name)] == '=')
			expand = ft_substr(data->envp[j], ft_strlen(name) + 1, ft_strlen(data->envp[j]));
		j++;
	}
	return (expand);
}

void	change_node(t_data *data, t_ast *c)
{

	
	int	i;
	char	*str;
	char	*temp;

	if (!ft_strcmp(c->str, "$?"))
	{
		free(c->str);
		c->str = ft_itoa(data->exit);
		return ;
	}

	i = 0;
	str = ft_strdup("");
	if (!str)
		data_destroy_exit(data, EXIT_FAILURE, "str", strerror(ENOMEM));
	// while (c->str[i])
	// {
	// 	if (c->str[i] == '\'')
	// 	{
	// 		i++;
	// 		while (c->str[i] && c->str[i] != '\'')
	// 		{
	// 			temp = malloc(2);
	// 			temp[0] = c->str[i];
	// 			temp[1] = '\0';
	// 			str = gnl_strjoin(str, temp);
	// 			free(temp);
	// 			i++;
	// 		}
	// 	}
	// 	else if (c->str[i] == '\"')
	// 	{
	// 		i++;
	// 		while (c->str[i] && c->str[i] != '\"')
	// 		{
	// 			if(c->str[i] == '$')
	// 				temp = do_expand(data, c->str, &i);	
	// 			else
	// 			{
	// 				temp = malloc(2);
	// 				temp[0] = c->str[i];
	// 				temp[1] = '\0';
	// 			}
	// 			str = gnl_strjoin(str, temp);
	// 			free(temp);
	// 			i++;
	// 		}
			
	// 	}
	// 	else if(c->str[i] == '$' && c->str[i + 1] && c->str[i + 1] == '?')
	// 	{
	// 		temp = ft_itoa(data->exit);
	// 		str = gnl_strjoin(str, temp);
	// 		free(temp);
	// 		i++;
	// 	}
	// 	else
	// 	{
	// 		temp = malloc(2);
	// 		temp[0] = c->str[i];
	// 		temp[1] = '\0';
	// 		str = gnl_strjoin(str, temp);
	// 		free(temp);
	// 	}
	// 	i++;
	// }
	while (c->str[i])
	{
		temp = malloc(2);
		temp[0] = c->str[i];
		temp[1] = '\0';
		str = gnl_strjoin(str, temp);
		free(temp);
		i++;
	}
	free(c->str);
	c->str = NULL;
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

void	exec_expr(t_data *data, t_ast *c, int in, int out)
{
	//change_word(data, c);
	//dprintf(2, "node: %s | int: %d | out: %d\n", c->str, data->in, data->out);
	if (is_builtin(c))
	{
		update_redir(data, c);
		close_useless_fds(data);
		builtin(data, c);
		data->in = in;
		data->out = out;
		if (c->prev && c->prev->token == PIPE)
			data_destroy_exit(data, EXIT_SUCCESS, NULL, NULL);
	}
	else if (c->prev && c->prev->token == PIPE)
	{
		update_redir(data, c);
		close_useless_fds(data);
		dup2(data->in, STDIN_FILENO);
		dup2(data->out, STDOUT_FILENO);
		exec_cmd(data, c);
	}
	else
		exec_cmd_fork(data, c);
}

