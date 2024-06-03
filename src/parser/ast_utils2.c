/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:07:29 by glemaire          #+#    #+#             */
/*   Updated: 2024/06/03 01:46:31 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_first_parenthesis(t_list **lex, int start)
{
	t_list	*c;

	c = ft_lstat(lex, start);
	if (((t_final *)c->content)->token == PAR_L)
		return (start);
	return (-1);
}

int	ft_findlast_pipe(t_list **lex, int start, int end)
{
	t_list	*c;
	int		i;
	int		n;
	int		count;
	int		token;

	c = ft_lstat(lex, start);
	i = 0;
	n = -1;
	count = 0;
	while (c && i < end - start + 1)
	{
		token = ((t_final *)c->content)->token;
		if (token == PAR_L)
			count++;
		if (token == PAR_R)
			count--;
		if (count == 0 && token == PIPE)
			n = start + i;
		i++;
		c = c->next;
	}
	return (n);
}

int	ft_findlast_andor(t_list **lex, int start, int end)
{
	t_list	*c;
	int		i;
	int		n;
	int		count;
	int		token;

	c = ft_lstat(lex, start);
	i = 0;
	n = -1;
	count = 0;
	while (c && i < end - start + 1)
	{
		token = ((t_final *)c->content)->token;
		if (token == PAR_L)
			count++;
		if (token == PAR_R)
			count--;
		if (count == 0 && (token == AND || token == OR))
			n = start + i;
		i++;
		c = c->next;
	}
	return (n);
}

t_ast	*fill_node(t_data *data, t_ast *c, int i)
{
	c->token = ((t_final *)ft_lstat(data->final_lex, i)->content)->token;
	c->str = ft_strdup(((t_final *)ft_lstat(data->final_lex, i)->content)->str);
	if (!c->str)
		reloop(data, EXIT_FAILURE, "c->str", strerror(ENOMEM));
	c->hdfd = -1;
	c->right = NULL;
	c->left = NULL;
	return (c);
}
