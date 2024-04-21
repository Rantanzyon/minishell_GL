/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_final.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:23:40 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/21 21:11:27 by glemaire         ###   ########.fr       */
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
	while (c)
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

void	create_lst_final(t_data *data)
{
	t_list	*cursor;

	cursor = *(data->lex);
	while (cursor)
	{
		if (((t_lex *)cursor->content)->pretok == REDIR_LEFT)
			is_redir_left(data, &cursor);
		else if (((t_lex *)cursor->content)->pretok == REDIR_RIGHT)
			is_redir_right(data, &cursor);
		else if (((t_lex *)cursor->content)->pretok == OR)
			is_or(data, &cursor);
		else if (((t_lex *)cursor->content)->pretok == AND)
			is_and(data, &cursor);
		else if (((t_lex *)cursor->content)->pretok == CHAR)
			is_word(data, &cursor);
		else if (((t_lex *)cursor->content)->pretok == EMPTY_STR)
			is_empty(data, &cursor);
		else if (((t_lex *)cursor->content)->pretok == PAR_L)
			is_par_l(data, &cursor);
		else if (((t_lex *)cursor->content)->pretok == PAR_R)
			is_par_r(data, &cursor);
		else
			cursor = cursor->next;
	}
}

void	lexer_final(t_data *data)
{
	remove_osef(data);
	remove_empty(data);
	data->final_lex = (t_list **)malloc(sizeof(t_list *));
	if (!data->final_lex)
		reloop(data, "data->final_lex", strerror(ENOMEM));
	*(data->final_lex) = NULL;
	create_lst_final(data);
}
