/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:03:48 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/21 19:23:05 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	syntax_error(t_data *data, char *str)
{
	char	*str2;
	char	*str3;

	str2 = ft_strjoin("syntax error near unexpected token `", str);
	str3 = ft_strjoin(str2, "\'");
	free(str);
	free(str2);
	if (!str3)
		reloop(data, "str3", strerror(ENOMEM));
	err_message(data, NULL, str3);
	free(str3);
	reloop(data, NULL, NULL);
}

static void	parenthesis_check(t_data *data, t_list *c)
{
	int		count;
	char	*str;

	count = 0;
	while (c)
	{
		if (((t_final *)c->content)->token == PAR_L)
			count++;
		else if (((t_final *)c->content)->token == PAR_R)
			count--;
		if (count < 0)
			break ;
		c = c->next;
	}
	if (count == 0)
		return ;
	if (count < 0)
		str = ft_strdup(")");
	else
		str = ft_strdup("(");
	if (!str)
		reloop(data, "str", strerror(ENOMEM));
	syntax_error(data, str);
}

void	syntax_check(t_data *data)
{
	t_list	*c;
	int		token;

	c = *(data->final_lex);
	while (c)
	{
		token = ((t_final *)c->content)->token;
		if (token == PIPE || token == AND || token == OR)
			syntax1(data, c);
		else if (token == LL_REDIR || token == L_REDIR || \
				token == RR_REDIR || token == R_REDIR)
			syntax2(data, c->next);
		else if (token == WORD)
			syntax3(data, c->next);
		else if (token == PAR_L)
			syntax4(data, c->next);
		else if (token == PAR_R)
			syntax5(data, c->next);
		c = c->next;
	}
	parenthesis_check(data, *(data->final_lex));
}
