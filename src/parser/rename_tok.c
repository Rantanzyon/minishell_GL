/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rename_tok.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 13:53:23 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/01 14:11:53 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rename_tok(t_data *data)
{
	t_list	*cursor;

	cursor = (*data->final_lex);
	while (cursor)
	{
		if (((t_final *)cursor->content)->token != PIPE && ((t_final *)cursor->content)->token != WORD && ((t_final *)cursor->content)->token != FILENAME)
			((t_final *)cursor->next->content)->token = FILENAME;
		cursor = cursor->next;
	}
}
