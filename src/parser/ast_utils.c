/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:07:29 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/02 16:47:47 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_lstchr(t_list **lex, int start, int end, int token)
{
	t_list	*c;
	int		i;

	c = ft_lstat(lex, start);
	i = 0;
	while (c && i < end - start)
	{
		if (token == WORD || token == PIPE)
		{
			if (((t_final *)c->content)->token == token)
				return (start + i);
		}
		else
		{
			if (((t_final *)c->content)->token != WORD && \
				((t_final *)c->content)->token != PIPE)
				return (start + i);
		}
		i++;
		c = c->next;
	}
	return (-1);
}

int	ft_findlast(t_list **lex, int start, int end, int token)
{
	t_list	*c;
	int		i;
	int		n;

	c = ft_lstat(lex, start);
	i = 0;
	n = -1;
	while (c && i < end - start)
	{
		if (((t_final *)c->content)->token == token)
			n = i;
		i++;
		c = c->next;
	}
	return (n);
}

t_ast	*fill_node(t_data *data, t_ast *c, int i)
{
	c->token = ((t_final *)ft_lstat(data->final_lex, i)->content)->token;
	c->str = ((t_final *)ft_lstat(data->final_lex, i)->content)->str;
	c->right = NULL;
	c->left = NULL;
	return (c);
}
