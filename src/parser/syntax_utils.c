/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 18:03:54 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/25 09:32:07 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	syntax1(t_data *data, t_list *c)
{
	char	*str;

	if (c == *(data->final_lex))
		str = ft_strdup(((t_final *)c->content)->str);
	else if (!c->next)
		str = ft_strdup("newline");
	else if (((t_final *)c->next->content)->token == PIPE)
		str = ft_strdup("|");
	else if (((t_final *)c->next->content)->token == AND)
		str = ft_strdup("&&");
	else if (((t_final *)c->next->content)->token == OR)
		str = ft_strdup("||");
	else
		return ;
	if (!str)
		reloop(data, EXIT_FAILURE, "str", strerror(ENOMEM));
	syntax_error(data, str);
}

void	syntax2(t_data *data, t_list *c)
{
	char	*str;

	if (!c)
		str = ft_strdup("newline");
	else if (((t_final *)c->content)->token != WORD)
		str = ft_strdup(((t_final *)c->content)->str);
	else
		return ;
	if (!str)
		reloop(data, EXIT_FAILURE, "str", strerror(ENOMEM));
	syntax_error(data, str);
}

void	syntax3(t_data *data, t_list *c)
{
	char	*str;

	if (c && ((t_final *)c->content)->token == PAR_L)
	{
		if (!c->next)
			str = ft_strdup("newline");
		else
			str = ft_strdup(((t_final *)c->next->content)->str);
	}
	else
		return ;
	if (!str)
		reloop(data, EXIT_FAILURE, "str", strerror(ENOMEM));
	syntax_error(data, str);
}

void	syntax4(t_data *data, t_list *c)
{
	char	*str;

	if (!c)
		return ;
	if (((t_final *)c->content)->token == PAR_R || \
		((t_final *)c->content)->token == PIPE || \
		((t_final *)c->content)->token == AND || \
		((t_final *)c->content)->token == OR)
		str = ft_strdup(((t_final *)c->content)->str);
	else
		return ;
	if (!str)
		reloop(data, EXIT_FAILURE, "str", strerror(ENOMEM));
	syntax_error(data, str);
}

void	syntax5(t_data *data, t_list *c)
{
	char	*str;

	if (!c)
		return ;
	if (((t_final *)c->content)->token == WORD || \
		((t_final *)c->content)->token == R_REDIR || \
		((t_final *)c->content)->token == RR_REDIR || \
		((t_final *)c->content)->token == L_REDIR || \
		((t_final *)c->content)->token == LL_REDIR || \
		((t_final *)c->content)->token == PAR_L)
		str = ft_strdup(((t_final *)c->content)->str);
	else
		return ;
	if (!str)
		reloop(data, EXIT_FAILURE, "str", strerror(ENOMEM));
	syntax_error(data, str);
}
