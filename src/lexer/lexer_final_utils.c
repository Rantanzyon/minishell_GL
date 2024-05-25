/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_final_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 21:18:13 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/25 09:30:09 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	is_redir_right(t_data *data, t_list **cursor)
{
	char	*word;

	if ((*cursor)->next && \
		((t_lex *)(*cursor)->next->content)->pretok == REDIR_RIGHT)
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
	if (!word)
		reloop(data, EXIT_FAILURE, "word", strerror(ENOMEM));
	*cursor = (*cursor)->next;
}

void	is_redir_left(t_data *data, t_list **cursor)
{
	char	*word;

	if ((*cursor)->next && \
		((t_lex *)(*cursor)->next->content)->pretok == REDIR_LEFT)
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
	if (!word)
		reloop(data, EXIT_FAILURE, "word", strerror(ENOMEM));
	*cursor = (*cursor)->next;
}

void	is_word(t_data *data, t_list **cursor)
{
	char	*word;
	char	*temp;

	word = ft_strdup("");
	if (!word)
		reloop(data, EXIT_FAILURE, "word", strerror(ENOMEM));
	while (*cursor && ((t_lex *)(*cursor)->content)->pretok == CHAR)
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
	ft_add_token(data, word, WORD);
}

void	is_empty(t_data *data, t_list **cursor)
{
	char	*word;

	word = ft_strdup("");
	if (!word)
		reloop(data, EXIT_FAILURE, "word", strerror(ENOMEM));
	ft_add_token(data, word, WORD);
	*cursor = (*cursor)->next;
}
