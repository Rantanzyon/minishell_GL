/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_destroy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:24:57 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/04 16:10:28 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_ast(t_ast *c)
{
	if (c->left)
		free_ast(c->left);
	if (c->right)
		free_ast(c->right);
	free(c->str);
	free(c);
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

void	data_destroy(t_data *data, char *err, char *str)
{
	if (err)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(err, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	if (data)
	{
		ft_lstclear(data->lex, free);
		free(data->lex);
		free_final_lex(data->final_lex);
		if (data->ast)
		{
			free_ast(*data->ast);
			free(data->ast);
		}
		free(data->input);
	}
}

void	data_destroy_exit(t_data *data, int status, char *err, char *str)
{
	data_destroy(data, err, str);
	free(data);
	rl_clear_history();
	exit(status);
}

void	reloop(t_data *data, char *err, char *str)
{
	data_destroy(data, err, str);
	ft_loop(data);
}
