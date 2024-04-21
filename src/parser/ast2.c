/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:47:59 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/21 00:09:19 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*malloc_node_ast(t_data *data)
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
	i = ft_findlast(data->final_lex, start, end, PIPE);
	//print_ast(*data->ast, 0);
	//printf("------- Enter PIPE\n");
	//printf("start = %d | end = %d\n", start, end);
	//printf("i = %d\n", i);
	//printf("------------------\n");
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
	//print_ast(*data->ast, 0);
	//printf("Enter AND/OR\n");
	//printf("start = %d | end = %d\n", start, end);
	//printf("i = %d\n", i);
	//printf("------------------\n");
	c = fill_node(data, c, i);
	c->right = malloc_node_ast(data);
	rec(data, c->right, i + 1, end);
	c->left = malloc_node_ast(data);
	rec(data, c->left, start, i - 1);
}

/* static void	fillnode_par(t_data *data, t_ast *c, int start, int end)
{
	int	i;

	i = ft_lstchr_par(data->final_lex, start, end);
	c = fill_node(data, c, i);
	free(((t_final *)ft_lstat(data->final_lex, i)->content));
	ft_lstdelnode(data->final_lex, i);
	c->right = (t_ast *)malloc(sizeof(t_ast));
	if (!c->right)
		reloop(data, "c->right", strerror(ENOMEM));
	rec(data, c->right, i, end - 1);
	c->left = (t_ast *)malloc(sizeof(t_ast));
	if (!c->left)
		reloop(data, "c->left", strerror(ENOMEM));
	rec(data, c->left, start, i);
} */

/* static void	fillnode_redir(t_data *data, t_ast *c, int start, int end)
{
	int	i;

	i = ft_lstchr(data->final_lex, start, end, R_REDIR);
	c = fill_node(data, c, i);
	c->left = malloc_node_ast(data);
	c->left = fill_node(data, c->left, i + 1);
	if (end - start > 1)
		c->right = malloc_node_ast(data);
	rec(data, c->right, start, end - 2);
} */

static void	fillnode_expression(t_data *data, t_ast *c, int start, int end)
{
	//printf("Enter expr\n");
	//printf("start = %d | end = %d\n", start, end);
	//printf("------------------\n");
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

/* static void	fillnode_word(t_data *data, t_ast *c, int start, int end)
{
	int	i;

	printf("Enter WORD\n");
	i = ft_lstchr(data->final_lex, start, end, WORD);
	printf("start = %d | WORD = %d | end = %d\n", start, i, end);
	c = fill_node(data, c, i);
	if (end - start > 0)
	{
		c->right = malloc_node_ast(data);
		rec(data, c->right, start + 1, end);
	}
} */

/* static void	fillnode_parenthesis(t_data *data, t_ast *c, int start, int end)
{
	//int	i;

	//i = find_last_par(data->final_lex, start, end);
	c = fill_node(data, c, start);
	c->left = malloc_node_ast(data);
	//printf("start = %d | WORD = %d | end = %d\n", start, i, end);
	rec(data, c->left, start + 1, end - 1);
} */



void	rec(t_data *data, t_ast *c, int start, int end)
{
	if (ft_findlast_andor(data->final_lex, start, end) != -1)
		fillnode_andor(data, c, start, end);
	else if (ft_findlast(data->final_lex, start, end, PIPE) != -1)
		fillnode_pipe(data, c, start, end);
	else if (ft_first_parenthesis(data->final_lex, start) != -1)
		rec(data, c, start + 1, end - 1);//fillnode_parenthesis(data, c, start, end);//rec(data, c, start + 1, end - 1);//fillnode_parenthesis(data, c, start, end);
	else
		fillnode_expression(data, c, start, end);
/* 	else if (ft_lstchr(data->final_lex, start, end, WORD) != -1)
		fillnode_word(data, c, start, end);
	else if (ft_lstchr(data->final_lex, start, end, R_REDIR) != -1)
		fillnode_redir(data, c, start, end); */

		
	//return ;
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
}
