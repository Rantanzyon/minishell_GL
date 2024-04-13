/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_destroy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:24:57 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/14 01:20:10 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	err_message(t_data *data, char *name, char *err)
{
	char	*str;
	
	str = ft_strdup("\033[31mminishell: ");
	if (name)
	{
		str = gnl_strjoin(str, name);
		str = gnl_strjoin(str, ": ");
	}
	if (err)
	{
		str = gnl_strjoin(str, err);
		str = gnl_strjoin(str, "\033[0m\n");
	}
	if (!str)
	{
		ft_putstr_fd(strerror(ENOMEM), STDERR_FILENO);
		data_destroy_exit(data, EXIT_FAILURE, NULL, NULL);
	}
	ft_putstr_fd(str, STDERR_FILENO);
	free(str);
}

void	close_fd(t_ast *c)
{
	if (c->hdfd != -1)
	{
		close(c->hdfd);
		c->hdfd = -1;
	}
}

void	free_ast(t_ast *c)
{
	if (c->left)
		free_ast(c->left);
	if (c->right)
		free_ast(c->right);
	close_fd(c);
	free(c->str);
	free(c);
}

void	free_env(t_list **a)
{
	t_list	*temp;
	t_list	*cursor;

	if (!a)
		return ;
	cursor = *a;
	while (cursor)
	{
		temp = cursor->next;
		free(cursor->content);
		free(cursor);
		cursor = temp;
	}
	free(a);
}

void	free_final_lex(t_list **a)
{
	t_list	*temp;
	t_list	*cursor;

	if (!a)
		return ;
	cursor = *a;
	while (cursor)
	{
		temp = cursor->next;
		free(((t_final *)cursor->content)->str);
		free(((t_final *)cursor->content));
		free(cursor);
		cursor = temp;
	}
	free(a);
}

void	data_destroy(t_data *data, char *name, char *err)
{
	if (err)
		err_message(data, name, err);
	if (data)
	{
		ft_lstclear(data->lex, free);
		free(data->lex);
		free_final_lex(data->final_lex);
		free_env(data->env);
		if (data->ast)
		{
			free_ast(*data->ast);
			free(data->ast);
		}
		free(data->input);
	}
}

void	data_destroy_exit(t_data *data, int status, char *name, char *err)
{
	data_destroy(data, name, err);
	free(data);
	clear_history();
	exit(status);
}

void	reloop(t_data *data, char *name, char *err)
{
	data_destroy(data, name, err);
	ft_loop(data);
}
