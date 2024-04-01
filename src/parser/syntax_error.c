/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:03:48 by glemaire          #+#    #+#             */
/*   Updated: 2024/03/31 23:58:48 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	syntax_error(t_data *data, t_list *c)
{
	if (!c)
		reloop(data, "minishell: syntax error near unexpected token `newline'");
	else if (((t_final *)c->content)->token == PIPE)
		reloop(data, "minishell: syntax error near unexpected token `|'");
	else if (((t_final *)c->content)->token == R_REDIR)
		reloop(data, "minishell: syntax error near unexpected token `>'");
	else if (((t_final *)c->content)->token == RR_REDIR)
		reloop(data, "minishell: syntax error near unexpected token `>>'");
	else if (((t_final *)c->content)->token == L_REDIR)
		reloop(data, "minishell: syntax error near unexpected token `<'");
	else if (((t_final *)c->content)->token == LL_REDIR)
		reloop(data, "minishell: syntax error near unexpected token `<<'");
}

void	syntax_check(t_data *data)
{
	t_list	*c;

	c = *(data->final_lex);
	if (c && ((t_final *)c->content)->token == PIPE)
		reloop(data, "minishell: syntax error near unexpected token `|'");
	while (c)
	{
		if (((t_final *)c->content)->token == PIPE)
		{
			if (c->next && ((t_final *)c->next->content)->token == PIPE)
				reloop(data,
					"minishell: syntax error near unexpected token `|'");
			else if (c->next == NULL)
				reloop(data,
					"minishell: syntax error near unexpected token `newline'");
		}
		if (((t_final *)c->content)->token != WORD && \
			((t_final *)c->content)->token != PIPE)
			syntax_error(data, c->next);
		c = c->next;
	}
}
