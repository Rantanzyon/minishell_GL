/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_final.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:23:40 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/14 00:29:51 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	is_pipe(t_data *data, t_list **cursor)
{
	char	*word;

	word = ft_strdup("|");
	if (!word)
		reloop(data, "word", strerror(ENOMEM));
	ft_add_token(data, word, PIPE);
	*cursor = (*cursor)->next;
}

static void	is_l_redir(t_data *data, t_list **cursor)
{
	char	*word;

	if ((*cursor)->next && ((t_lex *)(*cursor)->next->content)->c == '<')
	{
		word = ft_strdup("<<");
		if (!word)
			reloop(data, "word", strerror(ENOMEM));
		ft_add_token(data, word, LL_REDIR);
		*cursor = (*cursor)->next;
	}
	else
	{
		word = ft_strdup("<");
		if (!word)
			reloop(data, "word", strerror(ENOMEM));
		ft_add_token(data, word, L_REDIR);
	}
	*cursor = (*cursor)->next;
}

static void	is_r_redir(t_data *data, t_list **cursor)
{
	char	*word;

	if ((*cursor)->next && ((t_lex *)(*cursor)->next->content)->c == '>')
	{
		word = ft_strdup(">>");
		if (!word)
			reloop(data, "word", strerror(ENOMEM));
		ft_add_token(data, word, RR_REDIR);
		*cursor = (*cursor)->next;
	}
	else
	{
		word = ft_strdup(">");
		if (!word)
			reloop(data, "word", strerror(ENOMEM));
		ft_add_token(data, word, R_REDIR);
	}
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
		if (((t_lex *)cursor->content)->c == '<')
			is_l_redir(data, &cursor);
		else if (((t_lex *)cursor->content)->c == '>')
			is_r_redir(data, &cursor);
		else if (((t_lex *)cursor->content)->pretok == CHAR)
			is_word(data, &cursor);
		else if (((t_lex *)cursor->content)->pretok == EMPTY_STR)
			is_empty(data, &cursor);
		else if (((t_lex *)cursor->content)->c == '|')
			is_pipe(data, &cursor);
		else
			cursor = cursor->next;
	}
}
