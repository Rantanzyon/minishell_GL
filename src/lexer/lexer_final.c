/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_final.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:23:40 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/17 17:46:34 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	is_or(t_data *data, t_list **cursor)
{
	char	*word;

	if ((*cursor)->next && ((t_lex *)(*cursor)->next->content)->pretok == OR)
	{
		word = ft_strdup("||");
		ft_add_token(data, word, OR);
		*cursor = (*cursor)->next;
	}
	else
	{
		word = ft_strdup("|");
		ft_add_token(data, word, PIPE);
	}
	if (!word)
		reloop(data, "word", strerror(ENOMEM));
	*cursor = (*cursor)->next;
}

static void	is_and(t_data *data, t_list **cursor)
{
	char	*word;

	if ((*cursor)->next && ((t_lex *)(*cursor)->next->content)->pretok == AND)
	{
		word = ft_strdup("&&");
		ft_add_token(data, word, AND);
		*cursor = (*cursor)->next;
	}
	else
	{
		word = ft_strdup("&");
		ft_add_token(data, word, WORD);
	}
	if (!word)
		reloop(data, "word", strerror(ENOMEM));
	*cursor = (*cursor)->next;
}

static void	is_par_l(t_data *data, t_list **cursor)
{
	char	*word;

	word = ft_strdup("(");
	ft_add_token(data, word, PAR_L);
	if (!word)
		reloop(data, "word", strerror(ENOMEM));
	*cursor = (*cursor)->next;
}

static void	is_par_r(t_data *data, t_list **cursor)
{
	char	*word;

	word = ft_strdup(")");
	ft_add_token(data, word, PAR_R);
	if (!word)
		reloop(data, "word", strerror(ENOMEM));
	*cursor = (*cursor)->next;
}

static void	is_redir(t_data *data, t_list **cursor)
{
	char	*word;

	if (((t_lex *)(*cursor)->content)->c == '>')
	{
		if ((*cursor)->next && ((t_lex *)(*cursor)->next->content)->c == '>' \
							&& ((t_lex *)(*cursor)->next->content)->pretok == REDIR)
		{
			word = ft_strdup(">>");
			ft_add_token(data, word, RR_REDIR);
			*cursor = (*cursor)->next;
		}
		else
		{
			word = ft_strdup(">");
			ft_add_token(data, word, R_REDIR);
		}
	}
	else
	{
		if ((*cursor)->next && ((t_lex *)(*cursor)->next->content)->c == '<' \
							&& ((t_lex *)(*cursor)->next->content)->pretok == REDIR)
		{
			word = ft_strdup("<<");
			ft_add_token(data, word, LL_REDIR);
			*cursor = (*cursor)->next;
		}
		else
		{
			word = ft_strdup("<");
			ft_add_token(data, word, L_REDIR);
		}
	}
	if (!word)
		reloop(data, "word", strerror(ENOMEM));
	*cursor = (*cursor)->next;
}

static void	is_word(t_data *data, t_list **cursor)
{
	char	*word;
	char	*temp;

	word = ft_strdup("");
	if (!word)
		reloop(data, "word", strerror(ENOMEM));
	while (*cursor && ((t_lex *)(*cursor)->content)->pretok == CHAR)
	{
		temp = malloc(2);
		if (!temp)
		{
			free(word);
			reloop(data, "temp", strerror(ENOMEM));
		}
		temp[0] = ((t_lex *)(*cursor)->content)->c;
		temp[1] = '\0';
		word = gnl_strjoin(word, temp);
		free(temp);
		if (!word)
			reloop(data, "word", strerror(ENOMEM));
		*cursor = (*cursor)->next;
	}
	ft_add_token(data, word, WORD);
}

static void	is_empty(t_data *data, t_list **cursor)
{
	char	*word;

	word = ft_strdup("");
	if (!word)
		reloop(data, "word", strerror(ENOMEM));
	ft_add_token(data, word, WORD);
	*cursor = (*cursor)->next;
}

void	lexer_final(t_data *data)
{
	t_list	*cursor;

	remove_osef(data);
	remove_empty(data);
	data->final_lex = (t_list **)malloc(sizeof(t_list *));
	if (!data->final_lex)
		reloop(data, "data->final_lex", strerror(ENOMEM));
	*(data->final_lex) = NULL;
	cursor = *(data->lex);
	while (cursor)
	{
		if (((t_lex *)cursor->content)->pretok == REDIR)
			is_redir(data, &cursor);
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
