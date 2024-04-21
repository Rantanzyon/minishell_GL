/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rename_tok.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 13:53:23 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/17 18:07:03 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rename_tok(t_data *data)
{
	t_list	*cursor;

	cursor = (*data->final_lex);
	while (cursor)
	{
		if (((t_final *)cursor->content)->token == R_REDIR || \
			((t_final *)cursor->content)->token == RR_REDIR || \
			((t_final *)cursor->content)->token == L_REDIR || \
			((t_final *)cursor->content)->token == LL_REDIR)
			((t_final *)cursor->next->content)->token = FILENAME;
		cursor = cursor->next;
	}
}
