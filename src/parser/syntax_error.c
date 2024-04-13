/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:03:48 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/14 00:40:59 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	syntax_error(t_data *data, t_list *c)
{
	if (!c)
		reloop(data, NULL, SYNTX_ERR_NL);
	else if (((t_final *)c->content)->token == PIPE)
		reloop(data, NULL, SYNTX_ERR_PIPE);
	else if (((t_final *)c->content)->token == R_REDIR)
		reloop(data, NULL, SYNTX_ERR_R);
	else if (((t_final *)c->content)->token == RR_REDIR)
		reloop(data, NULL, SYNTX_ERR_RR);
	else if (((t_final *)c->content)->token == L_REDIR)
		reloop(data, NULL, SYNTX_ERR_L);
	else if (((t_final *)c->content)->token == LL_REDIR)
		reloop(data, NULL, SYNTX_ERR_LL);
}

void	syntax_check(t_data *data)
{
	t_list	*c;

	c = *(data->final_lex);
	if (c && ((t_final *)c->content)->token == PIPE)
		reloop(data, NULL, SYNTX_ERR_PIPE);
	while (c)
	{
		if (((t_final *)c->content)->token == PIPE)
		{
			if (c->next && ((t_final *)c->next->content)->token == PIPE)
				reloop(data, NULL, SYNTX_ERR_PIPE);
			else if (c->next == NULL)
				reloop(data, NULL, SYNTX_ERR_NL);
		}
		if (((t_final *)c->content)->token != WORD && \
			((t_final *)c->content)->token != FILENAME && \
			((t_final *)c->content)->token != PIPE)
			syntax_error(data, c->next);
		c = c->next;
	}
}
