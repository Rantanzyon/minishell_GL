/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_rec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 18:25:13 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/21 19:22:43 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_ast	*malloc_node_ast(t_data *data)
{
	t_ast	*node_ast;

	node_ast = (t_ast *)malloc(sizeof(t_ast));
	if (!node_ast)
		reloop(data, "node_ast", strerror(ENOMEM));
	return (node_ast);
}

static void	fillnode_pipe(t_data *data, t_ast *c, int start, int end)
{
	int	i;

	i = ft_findlast_pipe(data->final_lex, start, end);
	c = fill_node(data, c, i);
	c->right = malloc_node_ast(data);
	rec(data, c->right, i + 1, end);
	c->left = malloc_node_ast(data);
	rec(data, c->left, start, i - 1);
}

static void	fillnode_andor(t_data *data, t_ast *c, int start, int end)
{
	int	i;

	i = ft_findlast_andor(data->final_lex, start, end);
	c = fill_node(data, c, i);
	c->right = malloc_node_ast(data);
	rec(data, c->right, i + 1, end);
	c->left = malloc_node_ast(data);
	rec(data, c->left, start, i - 1);
}

static void	fillnode_expression(t_data *data, t_ast *c, int start, int end)
{
	c = fill_node(data, c, start++);
	if (c->token != WORD)
	{
		c->left = malloc_node_ast(data);
		c->left = fill_node(data, c->left, start++);
	}
	if (end - start >= 0)
	{
		c->right = malloc_node_ast(data);
		rec(data, c->right, start, end);
	}
}

void	rec(t_data *data, t_ast *c, int start, int end)
{
	if (ft_findlast_andor(data->final_lex, start, end) != -1)
		fillnode_andor(data, c, start, end);
	else if (ft_findlast_pipe(data->final_lex, start, end) != -1)
		fillnode_pipe(data, c, start, end);
	else if (ft_first_parenthesis(data->final_lex, start) != -1)
		rec(data, c, start + 1, end - 1);
	else
		fillnode_expression(data, c, start, end);
}
