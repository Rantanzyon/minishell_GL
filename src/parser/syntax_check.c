/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:03:48 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/21 02:53:22 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* static void	syntax_error(t_data *data, t_list *c)
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
	else if (((t_final *)c->content)->token == PAR_L)
		reloop(data, NULL, SYNTX_ERR_PARL);
	else if (((t_final *)c->content)->token == PAR_R)
		reloop(data, NULL, SYNTX_ERR_PARR);
} */



/* void	syntax_check(t_data *data)
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
		if (((t_final *)c->content)->token == LL_REDIR || \
			((t_final *)c->content)->token == L_REDIR || \
			((t_final *)c->content)->token == RR_REDIR || \
			((t_final *)c->content)->token == R_REDIR)
			syntax_error(data, c->next);
		c = c->next;
	}
	parenthesis_check(data, *(data->final_lex));
} */

static void	syntax_error(t_data *data, char *str)
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

static void	syntax1(t_data *data, t_list *c)
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
		reloop(data, "str", strerror(ENOMEM));
	syntax_error(data, str);
}

static void	syntax2(t_data *data, t_list *c)
{
	char	*str;
	
	if (!c)
		str = ft_strdup("newline");
	else if (((t_final *)c->content)->token != WORD)
		str = ft_strdup(((t_final *)c->content)->str);
	else
		return ;
	if (!str)
		reloop(data, "str", strerror(ENOMEM));
	syntax_error(data, str);
}

static void	syntax3(t_data *data, t_list *c)
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
		reloop(data, "str", strerror(ENOMEM));
	syntax_error(data, str);
}

static void	syntax4(t_data *data, t_list *c)
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
		reloop(data, "str", strerror(ENOMEM));
	syntax_error(data, str);
}

static void	syntax5(t_data *data, t_list *c)
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
		reloop(data, "str", strerror(ENOMEM));
	syntax_error(data, str);
}

void	parenthesis_check(t_data *data, t_list *c)
{
	int	count;
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
	//printf("%d\n", count);
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
