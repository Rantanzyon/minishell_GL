/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 23:01:59 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/01 08:49:51 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rec(t_data *data, t_ast *c, int start, int end);

static int	ft_lstchr(t_data *data, int start, int end, int token)
{
	t_list	*c;
	int		i;

	c = ft_lstat(data->final_lex, start);
	i = 0;
	while (c && i < end - start)
	{
		if (token == WORD || token == PIPE)
		{
			if (((t_final *)c->content)->token == token)
				return (start + i);
		}
		else
		{
			if (((t_final *)c->content)->token != WORD && \
				((t_final *)c->content)->token != PIPE)
				return (start + i);
		}
		i++;
		c = c->next;
	}
	//printf("lstchr2 = %s\n", ((t_final *)(*lex)->content)->str);
	return (-1);
}

void	fillnode_pipe(t_data *data, t_ast *c, int start, int end)
{
	int	i;

	i = ft_lstchr(data, start, end, PIPE);
	c->token = ((t_final *)(ft_lstat(data->final_lex, i))->content)->token;
	c->content = ((t_final *)(ft_lstat(data->final_lex, i))->content)->str;
	c->right = NULL;
	c->left = NULL;
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

void	fillnode_redir(t_data *data, t_ast *c, int start, int end)
{
	int	i;

	i = ft_lstchr(data, start, end, R_REDIR);
	c->token = ((t_final *)ft_lstat(data->final_lex, i)->content)->token;
	c->content = ((t_final *)ft_lstat(data->final_lex, i)->content)->str;
	c->right = NULL;
	c->left = NULL;
	c->left = (t_ast *)malloc(sizeof(t_ast));
	if (!c->left)
		reloop(data, "c->left: Allocation Failure (fillnode_redir)");
	c->left->token = \
	((t_final *)ft_lstat(data->final_lex, i)->next->content)->token;
	c->left->content = \
	((t_final *)ft_lstat(data->final_lex, i)->next->content)->str;
	c->left->right = NULL;
	c->left->left = NULL;
	ft_lstdelnode(data->final_lex, i);
	ft_lstdelnode(data->final_lex, i);
	if (end - start > 2)
	{
		c->right = (t_ast *)malloc(sizeof(t_ast));
		if (!c->right)
			reloop(data, "c->right: Allocation Failure (fillnode_redir)");
	}
	rec(data, c->right, start, end - 2);
}

void	fillnode_word(t_data *data, t_ast *c, int start, int end)
{
	int	i;

	i = ft_lstchr(data, start, end, WORD);
	c->token = ((t_final *)ft_lstat(data->final_lex, i)->content)->token;
	c->content = ((t_final *)ft_lstat(data->final_lex, i)->content)->str;
	c->right = NULL;
	c->left = NULL;
	ft_lstdelnode(data->final_lex, i);
	if (end - start > 1)
	{
		c->right = (t_ast *)malloc(sizeof(t_ast));
		if (!c->right)
			reloop(data, "c->right: Allocation Failure (fillnode_word)");
	}
	rec(data, c->right, start, end - 1);
}

/* void	rec(t_data *data, t_ast *c, int start, int end)
{
	if (ft_lstchr(data->final_lex, start, end, PIPE) != -1)
		fillnode_pipe(data, c, start, end);
	else if (ft_lstchr(data->final_lex, start, end, R_REDIR) != -1)
		fillnode_redir(data, c, start, end);
	else if (ft_lstchr(data->final_lex, start, end, WORD) != -1)
		fillnode_word(data, c, start, end);
	return ;
} */

void	rec(t_data *data, t_ast *c, int start, int end)
{
	if (ft_lstchr(data, start, end, PIPE) != -1)
		fillnode_pipe(data, c, start, end);
	else if (ft_lstchr(data, start, end, R_REDIR) != -1)
		fillnode_redir(data, c, start, end);
	else if (ft_lstchr(data, start, end, WORD) != -1)
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
	printf("str = %s\n", ((t_final *)(*data->final_lex)->content)->str);
	rec(data, c, 0, ft_lstsize(*(data->final_lex)));
	printf("str2 = %s\n", ((t_final *)(*data->final_lex)->content)->str);
}
