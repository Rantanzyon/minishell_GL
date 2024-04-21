/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_final_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 21:18:13 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/17 17:34:58 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_add_token(t_data *data, char *word, int token)
{
	t_list	*new_node;
	t_final	*new_struct;

	if (!word)
		return ;
	new_struct = (t_final *)malloc(sizeof(t_final));
	if (!new_struct)
	{
		free(word);
		reloop(data, "new_struct", strerror(ENOMEM));
	}
	new_struct->token = token;
	new_struct->str = word;
	new_node = ft_lstnew(new_struct);
	if (!new_node)
	{
		free(word);
		reloop(data, "new_node", strerror(ENOMEM));
	}
	ft_lstadd_back(data->final_lex, new_node);
}

void	remove_osef(t_data *data)
{
	t_list	*cursor;
	int		index;

	cursor = *data->lex;
	while (cursor)
	{
		if (((t_lex *)cursor->content)->pretok == OSEF)
		{
			index = ft_lstindex(data->lex, cursor);
			ft_lstdelnode(data->lex, index);
			cursor = ft_lstat(data->lex, index);
		}
		else
			cursor = cursor->next;
	}
}

void	remove_empty(t_data *data)
{
	t_list	*c;
	t_list	*prev;
	int		index;

	prev = NULL;
	c = *data->lex;
	while(c)
	{
		if (((t_lex *)c->content)->pretok == EMPTY_STR && \
			((prev && ((t_lex *)prev->content)->pretok == CHAR) || \
			(c->next && ((t_lex *)c->next->content)->pretok == CHAR)))
		{
			index = ft_lstindex(data->lex, c);
			ft_lstdelnode(data->lex, index);
			c = ft_lstat(data->lex, index);
		}
		else
		{
			prev = c;	
			c = c->next;
		}
	}
}
