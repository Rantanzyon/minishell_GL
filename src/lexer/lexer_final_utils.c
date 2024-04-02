/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_final_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 21:18:13 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/01 00:02:51 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_add_token(t_data *data, char *word, int token)
{
	t_list	*new_node;
	t_final	*new_struct;

	new_struct = (t_final *)malloc(sizeof(t_final));
	if (!new_struct)
	{
		free(word);
		reloop(data, "new_struct : Allocation failure (ft_add_token)");
	}
	new_struct->token = token;
	new_struct->str = word;
	new_node = ft_lstnew(new_struct);
	if (!new_node)
	{
		free(word);
		reloop(data, "new_node : Allocation failure (ft_add_token)");
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