/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:47:59 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/18 17:14:42 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fillnode_pipe(t_data *data, t_ast *c, int start, int end)
{
	int	i;

	i = ft_findlast(data->final_lex, start, end, PIPE);
	printf("Enter PIPE\n");
	printf("start = %d | PIPE = %d | end = %d\n", start, i, end);
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
}

static void	fillnode_andor(t_data *data, t_ast *c, int start, int end)
{
	int	i;

	i = ft_find_andor(data->final_lex, start, end);
	printf("Enter AND/OR\n");
	printf("start = %d | (AND/OR) = %d | end = %d\n", start, i, end);
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

static void	fillnode_redir(t_data *data, t_ast *c, int start, int end)
{
	int	i;

	i = ft_lstchr(data->final_lex, start, end, R_REDIR);
	c = fill_node(data, c, i);
	c->left = (t_ast *)malloc(sizeof(t_ast));
	if (!c->left)
		reloop(data, "c->left", strerror(ENOMEM));
	c->left = fill_node(data, c->left, i + 1);
	free(((t_final *)ft_lstat(data->final_lex, i)->content));
	ft_lstdelnode(data->final_lex, i);
	free(((t_final *)ft_lstat(data->final_lex, i)->content));
	ft_lstdelnode(data->final_lex, i);
	if (end - start > 2)
	{
		c->right = (t_ast *)malloc(sizeof(t_ast));
		if (!c->right)
			reloop(data, "c->right", strerror(ENOMEM));
	}
	rec(data, c->right, start, end - 2);
}

static void	fillnode_word(t_data *data, t_ast *c, int start, int end)
{
	int	i;

	printf("Enter WORD\n");
	i = ft_lstchr(data->final_lex, start, end, WORD);
	printf("start = %d | WORD = %d | end = %d\n", start, i, end);
	c = fill_node(data, c, i);
	free(((t_final *)ft_lstat(data->final_lex, i)->content));
	ft_lstdelnode(data->final_lex, i);
	if (end - start > 1)
	{
		c->right = (t_ast *)malloc(sizeof(t_ast));
		if (!c->right)
			reloop(data, "c->right", strerror(ENOMEM));
	}
	rec(data, c->right, start, end - 1);
}



void	rec(t_data *data, t_ast *c, int start, int end)
{
 	if (ft_lstchr_andor(data->final_lex, start, end) != -1)
		fillnode_andor(data, c, start, end);
	else if (ft_lstchr(data->final_lex, start, end, PIPE) != -1)
		fillnode_pipe(data, c, start, end);
	//else if (ft_lstchr_par(data->final_lex, start, end) != -1)
	//	fillnode_par(data, c, start, end);
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
		reloop(data, "data->ast", strerror(ENOMEM));
	c = (t_ast *)malloc(sizeof(t_ast));
	if (!c)
		reloop(data, "c", strerror(ENOMEM));
	*(data->ast) = c;
	rec(data, c, 0, ft_lstsize(*(data->final_lex)));
}
