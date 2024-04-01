/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quote_final.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:26:21 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/01 00:03:07 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_final_dq(t_list **cursor)
{
	(((t_lex *)(*cursor)->content))->pretok = OSEF;
	*cursor = (*cursor)->next;
	while (*cursor && (((t_lex *)(*cursor)->content))->pretok != DQ)
	{
		(((t_lex *)(*cursor)->content))->pretok = CHAR;
		*cursor = (*cursor)->next;
	}
	(((t_lex *)(*cursor)->content))->pretok = OSEF;
}

static void	ft_final_sq(t_list **cursor)
{
	(((t_lex *)(*cursor)->content))->pretok = OSEF;
	*cursor = (*cursor)->next;
	while (*cursor && (((t_lex *)(*cursor)->content))->pretok != SQ)
	{
		(((t_lex *)(*cursor)->content))->pretok = CHAR;
		*cursor = (*cursor)->next;
	}
	(((t_lex *)(*cursor)->content))->pretok = OSEF;
}

void	lexer_quote_final(t_data *data)
{
	t_list	*cursor;

	cursor = *(data->lex);
	while (cursor)
	{
		if (((t_lex *)cursor->content)->pretok == DQ)
			ft_final_dq(&cursor);
		if (((t_lex *)cursor->content)->pretok == SQ)
			ft_final_sq(&cursor);
		cursor = cursor->next;
	}
}
