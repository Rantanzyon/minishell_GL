/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_destroy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:24:57 by bbialy            #+#    #+#             */
/*   Updated: 2024/06/04 14:39:00 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_ast(t_ast *c)
{
	if (c->left)
		free_ast(c->left);
	if (c->right)
		free_ast(c->right);
	if (c->hdfd != -1)
	{
		close(c->hdfd);
		c->hdfd = -1;
	}
	free(c->str);
	free(c);
}

void	err_message(t_data *data, int status, char *name, char *err)
{
	char	*str;

	data->exit = status;
	str = ft_strjoin(RED, "minishell: ");
	if (name)
	{
		if (!ft_strcmp(err, NUMREQ))
			str = gnl_strjoin(str, "exit: ");
		str = gnl_strjoin(str, name);
		str = gnl_strjoin(str, ": ");
	}
	str = gnl_strjoin(str, err);
	str = gnl_strjoin(str, DEF);
	str = gnl_strjoin(str, "\n");
	if (!str)
	{
		ft_putstr_fd(strerror(ENOMEM), STDERR_FILENO);
		data_destroy_exit(data, EXIT_FAILURE, NULL, NULL);
	}
	ft_putstr_fd(str, STDERR_FILENO);
	free(str);
}

void	free_final_lex2(t_list **a)
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

void	free_fds(t_data *data)
{
	t_fds	*c;
	t_fds	*temp;
	
	if (!data->fds)
		return ;
	close_all_fds(data);
	c = *data->fds;
	while (c)
	{
		temp = c;
		c = c->next;
		free(temp);
	}
	free(data->fds);
}

void	data_destroy(t_data *data, int status, char *name, char *err)
{
	if (err)
		err_message(data, status, name, err);
	if (data)
	{
		data->exit = status;
		ft_lstclear(data->lex, free);
		free(data->lex);
		free_final_lex2(data->temp_final_lex);
		free_final_lex(data->final_lex);
		free_env(data->env);
		free_fds(data);
		if (data->ast)
		{
			free_ast(*data->ast);
			free(data->ast);
		}
		free(data->input);	
		if (data->in != STDIN_FILENO)
			close(data->in);
		if (data->out != STDOUT_FILENO)
			close(data->out);
		if (data->path)
			ft_free_array(data->path);
		if (data->args)
			ft_free_array(data->args);
	}
}

void	data_destroy_exit(t_data *data, int status, char *name, char *err)
{
	data_destroy(data, status, name, err);
	free(data);
	clear_history();
	exit(status);
}

void	reloop(t_data *data, int status, char *name, char *err)
{
	data_destroy(data, status, name, err);
	ft_loop(data);
}
