/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 18:39:28 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/01 00:09:00 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*exp_special(t_data *data, t_list *cursor, char nextok)
{
	int		i;
	char	*name;

	if (nextok == '$')
		name = ft_itoa(data->actual_pid);
	else
		name = ft_itoa(data->exit_status);
	if (!name)
		reloop(data, "name : Allocation failure (convert_exp)");
	i = ft_lstindex(data->lex, cursor);
	ft_lstdelnode(data->lex, i);
	ft_lstdelnode(data->lex, i);
	ft_lstadd_str(data, name, i);
	cursor = ft_lstat(data->lex, i);
	free(name);
	return (cursor);
}

void	lexer_expand(t_data *data)
{
	t_list	*cursor;
	t_lex	*nextnode;

	cursor = *(data->lex);
	while (cursor)
	{
		if (((t_lex *)cursor->content)->pretok == EXP && cursor->next)
		{
			nextnode = ((t_lex *)cursor->next->content);
			if (nextnode->pretok == DQ || nextnode->pretok == SPACE)
				((t_lex *)cursor->content)->pretok = CHAR;
			else if (nextnode->pretok == SQ)
				((t_lex *)cursor->content)->pretok = OSEF;
			else if (nextnode->c == '$' || nextnode->c == '?')
				cursor = exp_special(data, cursor, nextnode->c);
			else if (ft_isalpha(nextnode->c) || nextnode->c == '_')
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
