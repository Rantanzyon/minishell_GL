/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_destroy2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 19:00:30 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/23 12:17:04 by glemaire         ###   ########.fr       */
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

void	ft_free_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

