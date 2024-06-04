/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:26:16 by bbialy            #+#    #+#             */
/*   Updated: 2024/06/04 14:41:29 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_check_char(char c)
{
	if (c == '<')
		return (REDIR_LEFT);
	else if (c == '>')
		return (REDIR_RIGHT);
	else if (c == '"')
		return (DQ);
	else if (c == '\'')
		return (SQ);
	else if (c == ' ')
		return (ESPACE);
	else if (c == '&')
		return (AND);
	else if (c == '|')
		return (OR);
	else if (c == '(')
		return (PAR_L);
	else if (c == ')')
		return (PAR_R);
	else
		return (CHAR);
}

static void	ft_add_pretoken(t_data *data, char c, int pretok)
{
	t_lex	*cursor;
	t_list	*node;

	cursor = (t_lex *)malloc(sizeof(t_lex));
	if (!cursor)
		reloop(data, EXIT_FAILURE, "cursor", strerror(ENOMEM));
	cursor->c = c;
	cursor->pretok = pretok;
	node = ft_lstnew(cursor);
	if (!node)
		reloop(data, EXIT_FAILURE, "node", strerror(ENOMEM));
	ft_lstadd_back(data->lex, node);
}

void	check_empty_string(t_data *data, int i)
{
	char	cur;
	char	next;

	cur = data->input[i];
	next = data->input[i + 1];
	if ((cur == '\'' && next && next == '\'') || \
		(cur == '"' && next && next == '"'))
		ft_add_pretoken(data, 'E', EMPTY_STR);
}

static void	lexer_fill_char(t_data *data)
{
	int		i;
	char	c;
	int		pretok;

	i = 0;
	data->lex = (t_list **)malloc(sizeof(t_list *));
	if (!data->lex)
		reloop(data, EXIT_FAILURE, "data->lex", strerror(ENOMEM));
	*(data->lex) = NULL;
	while (data->input[i])
	{
		c = data->input[i];
		pretok = ft_check_char(data->input[i]);
		ft_add_pretoken(data, c, pretok);
		//check_empty_string(data, i);
		i++;
	}
}

void	lexer(t_data *data)
{
	lexer_fill_char(data);
	//print_lex(data);
	// lexer_quote(data);
	// lexer_expand(data);
	// lexer_quote_final(data);
	lexer_temp(data);
	lexer_final(data);
	if (!*data->final_lex)
	 	reloop(data, EXIT_SUCCESS, NULL, NULL);
	//print_lst(data->final_lex);
}
