/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 23:01:59 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/01 19:00:17 by glemaire         ###   ########.fr       */
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
	//printf("start = %d, end = %d, token = %d\n", start, end, token);
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
				((t_final *)c->content)->token != FILENAME && \
				((t_final *)c->content)->token != PIPE)
				return (start + i);
		}
		i++;
		c = c->next;
	}
	//printf("start = %d, end = %d, token = %d\n", start, end, token);
	//printf("lstchr2 = %s\n", ((t_final *)c->content)->str);
	return (-1);
}

static int	ft_lastpipe(t_data *data, int start, int end)
{
	t_list	*c;
	int		i;
	int		n;

	c = ft_lstat(data->final_lex, start);
	i = 0;
	n = -1;
	while (c && i < end - start)
	{
		if (((t_final *)c->content)->token == PIPE)
			n = i;
		i++;
		c = c->next;
	}
	//printf("start = %d, end = %d, token = %d\n", start, end, token);
	//printf("lstchr2 = %s\n", ((t_final *)c->content)->str);
	return (n);
	
}

/* void	fillnode_pipe(t_data *data, t_ast *c, int start, int end)
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
} */

void	fillnode_pipe(t_data *data, t_ast *c, int start, int end)
{
	int	i;
	printf("enter pipe\n");

	c->token = PIPE;
	c->left = NULL;
	c->right = NULL;
	c->str = ft_strdup("|");
	if (!c->str)
		reloop(data, "new_node->str: Allocation Failure (fillnode_word)");
	
	i = ft_lastpipe(data, start, end);
	c->right = (t_ast *)malloc(sizeof(t_ast));
	if (!c->right)
		reloop(data, "c->right: Allocation Failure (fillnode_pipe)");
	c->right->left = NULL;
	c->right->right = NULL;
	rec(data, c->right, i + 1, end);
	c->left = (t_ast *)malloc(sizeof(t_ast));
	if (!c->left)
		reloop(data, "c->left: Allocation Failure (fillnode_pipe)");
	c->left->left = NULL;
	c->left->right = NULL;
	rec(data, c->left, start, i);
}

/* void	manage_expr(t_data *data, t_ast *c, int start, int end)
{
	int	i;
	t_list	*temp;

	i = 0;
	
	printf("enter expr\n");
	while (i < end - start)
	{
		printf("REDIR | i = %d, start = %d, end = %d\n", i, start, end);
		temp = ft_lstat(data->final_lex, i + start);
		//printf("tok = %d\n", ((t_final *)temp->content)->token);
		if (((t_final *)temp->content)->token != PIPE && ((t_final *)temp->content)->token != WORD && ((t_final *)temp->content)->token != FILENAME)
		{
			
			c->token = ((t_final *)temp->content)->token;
			c->left = NULL;
			c->right = NULL;
			c->str = ft_strdup(((t_final *)temp->content)->str);
			if (!c->str)
				reloop(data, "new_node->str: Allocation Failure (fillnode_word)");

			c->left = (t_ast *)malloc(sizeof(t_ast));
			if (!c->left)
				reloop(data, "new_node: Allocation Failure (manage_expr)");
			c->left->token = ((t_final *)temp->next->content)->token;
			c->left->left = NULL;
			c->left->right = NULL;
			c->left->str = ft_strdup(((t_final *)temp->next->content)->str);
			if (!c->left->str)
				reloop(data, "new_node->str: Allocation Failure (fillnode_word)");

			if (ft_lstchr(data, start + i + 1, end, R_REDIR) != -1)
			{
				printf("if redir | i = %d, start = %d, end = %d\n", i, start, end);
				c->right = (t_ast *)malloc(sizeof(t_ast));
				if (!c->right)
					reloop(data, "new_node: Allocation Failure (manage_expr)");
				c->right->right = NULL;
				c->right->left = NULL;
				c->right->str = NULL;
				c = c->right;
			}
			
		}
		i++;
	}


} */

void	

void	manage_expr(t_data *data, t_ast *c, int start, int end)
{
	int	i;
	t_list	*temp;

	printf("enter expr\n");
	i = ft_lstchr(data, start, end, R_REDIR);
	temp = ft_lstat(data->final_lex, i);
	c->token = ((t_final *)temp->content)->token;
	c->left = NULL;
	c->right = NULL;
	c->str = ft_strdup(((t_final *)temp->content)->str);
	if (!c->str)
		reloop(data, "new_node->str: Allocation Failure (fillnode_word)");

	c->left = (t_ast *)malloc(sizeof(t_ast));
	if (!c->left)
		reloop(data, "new_node: Allocation Failure (manage_expr)");
	c->left->token = ((t_final *)temp->next->content)->token;
	c->left->left = NULL;
	c->left->right = NULL;
	c->left->str = ft_strdup(((t_final *)temp->next->content)->str);
	if (!c->left->str)
		reloop(data, "new_node->str: Allocation Failure (fillnode_word)");

	if (ft_lstchr(data, i + 1, end, R_REDIR) != -1)
	{
		c->right = (t_ast *)malloc(sizeof(t_ast));
		if (!c->right)
			reloop(data, "new_node: Allocation Failure (manage_expr)");
		c->right->right = NULL;
		c->right->left = NULL;
		c->right->str = NULL;
		manage_expr(data, c->right, i + 1, end);
	}
	rec()
}





void	manage_word(t_data *data, t_ast *c, int start, int end)
{
	t_list	*temp;
	int		i;

	printf("enter word\n");
	i = ft_lstchr(data, start, end, WORD);
	temp = ft_lstat(data->final_lex, i);

	c->token = ((t_final *)temp->content)->token;
	c->left = NULL;
	c->right = NULL;
	c->str = ft_strdup(((t_final *)temp->content)->str);
	if (!c->str)
		reloop(data, "new_node->str: Allocation Failure (fillnode_word)");

	if (ft_lstchr(data, i + 1, end, WORD) != -1)
	{
		c->right = (t_ast *)malloc(sizeof(t_ast));
		if (!c->right)
			reloop(data, "new_node: Allocation Failure (manage_expr)");
		c->right->right = NULL;
		c->right->left = NULL;
		c->right->str = NULL;
		manage_word(data, c->right, i + 1, end);
	}
}

void	rec(t_data *data, t_ast *c, int start, int end)
{
	printf("rec | start = %d, end = %d\n", start, end);
	if (ft_lstchr(data, start, end, PIPE) != -1)
		fillnode_pipe(data, c, start, end);
	if (ft_lstchr(data, start, end, R_REDIR) != -1)
		manage_expr(data, c, start, end);
	if (ft_lstchr(data, start, end, WORD) != -1)
		manage_word(data, c, start, end);
	//printf("FFEUR\n");
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

	/* data->ast = (t_ast **)malloc(sizeof(t_ast *));
	if (!data->ast)
		reloop(data, "data->ast: Allocation Failure (ast)");
	*(data->ast) = NULL; */
	//*(data->ast) = (t_ast *)malloc(sizeof(t_ast));
	//if (!*(data->ast))
		//reloop(data, "*(data->ast): Allocation Failure (ast)");
	
	//c->right = NULL;
	//c->left = NULL;
	//printf("str = %s\n", ((t_final *)(*data->final_lex)->content)->str);
	rec(data, c, 0, ft_lstsize(*(data->final_lex)));
	//printf("str2 = %s\n", ((t_final *)(*data->final_lex)->content)->str);
}
