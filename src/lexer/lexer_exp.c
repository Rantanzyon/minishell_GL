/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 18:39:28 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/05 09:28:06 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*exp_special(t_data *data, t_list *cursor, char nextok)
{
	int		i;
	char	*name;

	i = ft_lstindex(data->lex, cursor);
	if (nextok == '$')
		name = ft_itoa(data->actual_pid);
	else if (nextok == '?')
		name = ft_itoa(data->exec->exit_status);
	else
		name = ft_strdup(data->argv[0] + 2);
	if (!name)
		reloop(data, "name : Allocation failure (convert_exp)");
	free(((t_lex *)ft_lstat(data->lex, i)->content));
	ft_lstdelnode(data->lex, i);
	free(((t_lex *)ft_lstat(data->lex, i)->content));
	ft_lstdelnode(data->lex, i);
	ft_lstadd_str(data, name, i);
	cursor = ft_lstat(data->lex, i);
	free(name);
	return (cursor);
}

int	next_ignore_exp(t_lex *next)
{
	char	special_char[7];

	special_char[0] = '~';
	special_char[1] = '%';
	special_char[2] = '+';
	special_char[3] = '^';
	special_char[4] = '=';
	special_char[5] = ':';
	special_char[6] = ',';
	if (ft_strchr(special_char, next->c))
		return (1);
	if (next->pretok == ESPACE || next->pretok == DQ)
		return (1);
	return (0);
}

void	lexer_expand(t_data *data)
{
	t_list	*cursor;
	t_lex	*nextc;

	cursor = *(data->lex);
	while (cursor)
	{
		if (((t_lex *)cursor->content)->pretok == EXP && cursor->next)
		{
			nextc = ((t_lex *)cursor->next->content);
			if (next_ignore_exp(nextc))
				((t_lex *)cursor->content)->pretok = CHAR;
			else if (nextc->pretok == SQ)
				((t_lex *)cursor->content)->pretok = OSEF;
			else if (nextc->c == '$' || nextc->c == '?' || nextc->c == '0')
				cursor = exp_special(data, cursor, nextc->c);
			else if (ft_isalpha(nextc->c) || nextc->c == '_')
				cursor = exp_var(data, cursor);
			else
				((t_lex *)cursor->content)->pretok = CHAR;
		}
		else if (((t_lex *)cursor->content)->pretok == EXP && !cursor->next)
			((t_lex *)cursor->content)->pretok = CHAR;
		else
			cursor = cursor->next;
	}
}
