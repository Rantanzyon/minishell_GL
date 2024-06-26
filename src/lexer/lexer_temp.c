/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_temp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:23:40 by bbialy            #+#    #+#             */
/*   Updated: 2024/05/26 16:54:52 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_add_token(t_data *data, t_list **lst, char *word, int token)
{
	t_list	*new_node;
	t_final	*new_struct;

	if (!word)
		return ;
	new_struct = (t_final *)malloc(sizeof(t_final));
	if (!new_struct)
	{
		free(word);
		reloop(data, EXIT_FAILURE, "new_struct", strerror(ENOMEM));
	}
	new_struct->token = token;
	new_struct->str = word;
	new_node = ft_lstnew(new_struct);
	if (!new_node)
	{
		free(word);
		reloop(data, EXIT_FAILURE, "new_node", strerror(ENOMEM));
	}
	ft_lstadd_back(lst, new_node);
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

void	is_dq(t_data *data, t_list **cursor)
{
	char	*word;
	char	*temp;

	word = ft_strdup("\"");
	if (!word)
		reloop(data, EXIT_FAILURE, "word", strerror(ENOMEM));
	*cursor = (*cursor)->next;
	while (*cursor && ((t_lex *)(*cursor)->content)->pretok != DQ)
	{
		temp = malloc(2);
		if (!temp)
		{
			free(word);
			reloop(data, EXIT_FAILURE, "temp", strerror(ENOMEM));
		}
		temp[0] = ((t_lex *)(*cursor)->content)->c;
		temp[1] = '\0';
		word = gnl_strjoin(word, temp);
		free(temp);
		if (!word)
			reloop(data, EXIT_FAILURE, "word", strerror(ENOMEM));
		*cursor = (*cursor)->next;
	}
	if (*cursor && ((t_lex *)(*cursor)->content)->pretok == DQ)
	{
		word = gnl_strjoin(word, "\"");
		*cursor = (*cursor)->next;
	}
	ft_add_token(data, data->temp_final_lex, word, WORD);
}

void	is_sq(t_data *data, t_list **cursor)
{
	char	*word;
	char	*temp;

	word = ft_strdup("\'");
	if (!word)
		reloop(data, EXIT_FAILURE, "word", strerror(ENOMEM));
	*cursor = (*cursor)->next;
	while (*cursor && ((t_lex *)(*cursor)->content)->pretok != SQ)
	{
		temp = malloc(2);
		if (!temp)
		{
			free(word);
			reloop(data, EXIT_FAILURE, "temp", strerror(ENOMEM));
		}
		temp[0] = ((t_lex *)(*cursor)->content)->c;
		temp[1] = '\0';
		word = gnl_strjoin(word, temp);
		free(temp);
		if (!word)
			reloop(data, EXIT_FAILURE, "word", strerror(ENOMEM));
		*cursor = (*cursor)->next;
	}
	if (*cursor && ((t_lex *)(*cursor)->content)->pretok == SQ)
	{
		word = gnl_strjoin(word, "\'");
		*cursor = (*cursor)->next;
	}
	ft_add_token(data, data->temp_final_lex, word, WORD);
}

void	is_espace(t_data *data, t_list **cursor)
{
	char	*word;

	word = ft_strdup("ESPACE");
	if (!word)
		reloop(data, EXIT_FAILURE, "word", strerror(ENOMEM));
	while (*cursor && ((t_lex *)(*cursor)->content)->pretok == ESPACE)
		*cursor = (*cursor)->next;
	ft_add_token(data, data->temp_final_lex, word, ESPACE);
}

void	create_lst_temp(t_data *data)
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
		else if (((t_lex *)cursor->content)->pretok == DQ)
			is_dq(data, &cursor);
		else if (((t_lex *)cursor->content)->pretok == SQ)
			is_sq(data, &cursor);
		else if (((t_lex *)cursor->content)->pretok == CHAR)
			is_word(data, &cursor);
		else if (((t_lex *)cursor->content)->pretok == ESPACE)
			is_espace(data, &cursor);
		else if (((t_lex *)cursor->content)->pretok == PAR_L)
			is_par_l(data, &cursor);
		else if (((t_lex *)cursor->content)->pretok == PAR_R)
			is_par_r(data, &cursor);
		else
			cursor = cursor->next;
	}
}




void	lexer_temp(t_data *data)
{
	data->temp_final_lex = (t_list **)malloc(sizeof(t_list *));
	if (!data->temp_final_lex)
		reloop(data, EXIT_FAILURE, "data->final_lex", strerror(ENOMEM));
	*(data->temp_final_lex) = NULL;
	create_lst_temp(data);
}
