/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:47:59 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/22 22:06:32 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_prev_node_data(t_data *data, t_ast *c, int prev)
{
	c->prev_node = prev;
	if (c->right)
		fill_prev_node_data(data, c->right, c->token);
	if (c->left)
		fill_prev_node_data(data, c->left, c->token);
}

void	ast(t_data *data)
{
	t_ast	*c;

	data->ast = (t_ast **)malloc(sizeof(t_ast *));
	if (!data->ast)
		reloop(data, "data->ast", strerror(ENOMEM));
	c = (t_ast *)malloc(sizeof(t_ast));
	if (!c)
		reloop(data, "c", strerror(ENOMEM));
	*(data->ast) = c;
	rec(data, c, 0, ft_lstsize(*(data->final_lex)) - 1);
	fill_prev_node_data(data, *data->ast, START);
}
