/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:47:59 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/02 16:48:57 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fillnode_pipe(t_data *data, t_ast *c, int start, int end)
{
	int	i;

	i = ft_findlast(data->final_lex, start, end, PIPE);
	c = fill_node(data, c, i);
	free(((t_final *)ft_lstat(data->final_lex, i)->content));
	ft_lstdelnode(data->final_lex, i);
	c->right = (t_ast *)malloc(sizeof(t_ast));
	if (!c->right)
		reloop(data, "c->right: Allocation Failure (fillnode_pipe)");
	rec(data, c->right, i, end - 1);
	c->left = (t_ast *)malloc(sizeof(t_ast));
	if (!c->left)
		reloop(data, "c->left: Allocation Failure (fillnode_pipe)");
	rec(data, c->left, start, i);
}

static void	fillnode_redir(t_data *data, t_ast *c, int start, int end)
{
	int	i;

	i = ft_lstchr(data->final_lex, start, end, R_REDIR);
	c = fill_node(data, c, i);
	c->left = (t_ast *)malloc(sizeof(t_ast));
	if (!c->left)
		reloop(data, "c->left: Allocation Failure (fillnode_redir)");
	c->left = fill_node(data, c->left, i + 1);
	free(((t_final *)ft_lstat(data->final_lex, i)->content));
	ft_lstdelnode(data->final_lex, i);
	free(((t_final *)ft_lstat(data->final_lex, i)->content));
	ft_lstdelnode(data->final_lex, i);
	if (end - start > 2)
	{
		c->right = (t_ast *)malloc(sizeof(t_ast));
		if (!c->right)
			reloop(data, "c->right: Allocation Failure (fillnode_redir)");
	}
	rec(data, c->right, start, end - 2);
}

static void	fillnode_word(t_data *data, t_ast *c, int start, int end)
{
	int	i;

	i = ft_lstchr(data->final_lex, start, end, WORD);
	c = fill_node(data, c, i);
	free(((t_final *)ft_lstat(data->final_lex, i)->content));
	ft_lstdelnode(data->final_lex, i);
	if (end - start > 1)
	{
		c->right = (t_ast *)malloc(sizeof(t_ast));
		if (!c->right)
			reloop(data, "c->right: Allocation Failure (fillnode_word)");
	}
	rec(data, c->right, start, end - 1);
}

void	rec(t_data *data, t_ast *c, int start, int end)
{
	if (ft_lstchr(data->final_lex, start, end, PIPE) != -1)
		fillnode_pipe(data, c, start, end);
	else if (ft_lstchr(data->final_lex, start, end, R_REDIR) != -1)
		fillnode_redir(data, c, start, end);
	else if (ft_lstchr(data->final_lex, start, end, WORD) != -1)
		fillnode_word(data, c, start, end);
	return ;
}

void	ast(t_data *data)
{
	t_ast	*c;

	data->ast = (t_ast **)malloc(sizeof(t_ast *));
	if (!data->ast)
		reloop(data, "c->ast2: Allocation Failure (ast2)");
	c = (t_ast *)malloc(sizeof(t_ast));
	if (!c)
		reloop(data, "c: Allocation Failure (ast2)");
	*(data->ast) = c;
	rec(data, c, 0, ft_lstsize(*(data->final_lex)));
}
