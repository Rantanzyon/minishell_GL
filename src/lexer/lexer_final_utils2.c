/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_final_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 20:52:53 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/26 16:51:18 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	is_or(t_data *data, t_list **cursor)
{
	char	*word;

	if ((*cursor)->next && ((t_lex *)(*cursor)->next->content)->pretok == OR)
	{
		word = ft_strdup("||");
		ft_add_token(data, data->temp_final_lex, word, OR);
		*cursor = (*cursor)->next;
	}
	else
	{
		word = ft_strdup("|");
		ft_add_token(data, data->temp_final_lex, word, PIPE);
	}
	if (!word)
		reloop(data, EXIT_FAILURE, "word", strerror(ENOMEM));
	*cursor = (*cursor)->next;
}

void	is_and(t_data *data, t_list **cursor)
{
	char	*word;

	if ((*cursor)->next && ((t_lex *)(*cursor)->next->content)->pretok == AND)
	{
		word = ft_strdup("&&");
		ft_add_token(data, data->temp_final_lex, word, AND);
		*cursor = (*cursor)->next;
	}
	else
	{
		word = ft_strdup("&");
		ft_add_token(data, data->temp_final_lex, word, WORD);
	}
	if (!word)
		reloop(data, EXIT_FAILURE, "word", strerror(ENOMEM));
	*cursor = (*cursor)->next;
}

void	is_par_l(t_data *data, t_list **cursor)
{
	char	*word;

	word = ft_strdup("(");
	ft_add_token(data, data->temp_final_lex, word, PAR_L);
	if (!word)
		reloop(data, EXIT_FAILURE, "word", strerror(ENOMEM));
	*cursor = (*cursor)->next;
}

void	is_par_r(t_data *data, t_list **cursor)
{
	char	*word;

	word = ft_strdup(")");
	ft_add_token(data, data->temp_final_lex, word, PAR_R);
	if (!word)
		reloop(data, EXIT_FAILURE, "word", strerror(ENOMEM));
	*cursor = (*cursor)->next;
}
