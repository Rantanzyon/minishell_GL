/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:47:59 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/31 23:46:59 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_prev_node(t_data *data, t_ast *c, int prev)
{
	c->prev_node = prev;
	if (c->right)
	{
		if (c->token != PIPE && c->token != AND && c->token != OR)
			fill_prev_node(data, c->right, prev);
		else
			fill_prev_node(data, c->right, c->token);
	}
	if (c->left)
	{
		if (c->token != PIPE && c->token != AND && c->token != OR)
			fill_prev_node(data, c->left, prev);
		else
			fill_prev_node(data, c->left, c->token);
	}
}

void	prev_node(t_ast *c, t_ast *previous_node)
{
	c->prev = previous_node;
	if (c->right)
		prev_node(c->right, c);
	if (c->left)
		prev_node(c->left, c);
}

void	ast(t_data *data)
{
	t_ast	*c;

	data->ast = (t_ast **)malloc(sizeof(t_ast *));
	if (!data->ast)
		reloop(data, EXIT_FAILURE, "data->ast", strerror(ENOMEM));
	c = (t_ast *)malloc(sizeof(t_ast));
	if (!c)
		reloop(data, EXIT_FAILURE, "c", strerror(ENOMEM));
	*(data->ast) = c;
	rec(data, c, 0, ft_lstsize(*(data->final_lex)) - 1);
	fill_prev_node(data, *data->ast, START);
	prev_node(*data->ast, NULL);
	
}
