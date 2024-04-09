/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_destroy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:24:57 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/07 13:57:01 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	data_destroy(t_data *data, char *err)
{
	if (err)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(err);
	}
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

void	data_destroy_exit(t_data *data, int status, char *err)
{
	data_destroy(data, err);
	free(data);
	clear_history();
	exit(status);
}

void	reloop(t_data *data, char *err)
{
	data_destroy(data, err);
	ft_loop(data);
}
