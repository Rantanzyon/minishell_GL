/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:26:28 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/14 00:36:16 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	d_quote(t_data *data, t_list **cursor, int *active)
{
	*active = 1;
	*cursor = (*cursor)->next;
	while (*cursor && ((t_lex *)(*cursor)->content)->c != '"')
	{
		if (((t_lex *)(*cursor)->content)->c == '$')
			((t_lex *)(*cursor)->content)->pretok = EXP;
		else if (((t_lex *)(*cursor)->content)->pretok != EMPTY_STR)
			((t_lex *)(*cursor)->content)->pretok = CHAR;
		*cursor = (*cursor)->next;
	}
	if (*cursor)
		*active = 0;
	else
		reloop(data, NULL, SYNTX_ERR_UNCLOSED);
}

static void	s_quote(t_data *data, t_list **cursor, int *active)
{
	*active = 1;
	*cursor = (*cursor)->next;
	while (*cursor && ((t_lex *)(*cursor)->content)->c != '\'')
	{
		if (((t_lex *)(*cursor)->content)->pretok != EMPTY_STR)
			((t_lex *)(*cursor)->content)->pretok = CHAR;
		*cursor = (*cursor)->next;
	}
	if (*cursor)
		*active = 0;
	else
		reloop(data, NULL, SYNTX_ERR_UNCLOSED);
}

void	lexer_quote(t_data *data)
{
	t_list	*cursor;
	int		active;

	active = 0;
	cursor = *(data->lex);
	while (cursor)
	{
		if (((t_lex *)cursor->content)->c == '"' && active == 0)
			d_quote(data, &cursor, &active);
		else if (((t_lex *)cursor->content)->c == '\'' && active == 0)
			s_quote(data, &cursor, &active);
		cursor = cursor->next;
	}
}
