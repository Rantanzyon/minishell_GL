#include "minishell.h"

void	rec2(t_data *data, t_ast *c, int start, int end);

static int	ft_lstchr(t_list **lex, int start, int end, int token)
{
	t_list	*c;
	int	i;

	c = ft_lstat(lex, start);
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
			if (((t_final *)c->content)->token != WORD && ((t_final *)c->content)->token != PIPE)
				return (start + i);
		}
		i++;
		c = c->next;
	}
	return (-1);
}

static int	ft_findlast(t_list **lex, int start, int end, int token)
{
	t_list	*c;
	int	i;
	int	n;

	c = ft_lstat(lex, start);
	i = 0;
	n = -1;
	//printf("enter findlast\n");
	while (c && i < end - start)
	{
		if (((t_final *)c->content)->token == token)
			n = i;
		i++;
		c = c->next;
	}
	return (n);
}

t_ast	*fill_node(t_data *data, t_ast *c, int i)
{
	c->token = ((t_final *)ft_lstat(data->final_lex, i)->content)->token;
	c->str = ((t_final *)ft_lstat(data->final_lex, i)->content)->str;
	c->right = NULL;
	c->left = NULL;
	return (c);
}

void	fillnode_pipe(t_data *data, t_ast *c, int start, int end)
{
	int	i;

	//printf("enter pipe\n");
	i = ft_findlast(data->final_lex, start, end, PIPE);
	c = fill_node(data, c, i);
	free(((t_final *)ft_lstat(data->final_lex, i)->content));
	ft_lstdelnode(data->final_lex, i);
	c->right = (t_ast *)malloc(sizeof(t_ast));
	if (!c->right)
		reloop(data, "c->right: Allocation Failure (fillnode_pipe)");
	rec2(data, c->right, i, end - 1);
	c->left = (t_ast *)malloc(sizeof(t_ast));
	if (!c->left)
		reloop(data, "c->left: Allocation Failure (fillnode_pipe)");
	rec2(data, c->left, start, i);
}

void	fillnode_redir(t_data *data, t_ast *c, int start, int end)
{
	int	i;

	//printf("enter redir\n");
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
	rec2(data, c->right, start, end - 2);
}

void	fillnode_word(t_data *data, t_ast *c, int start, int end)
{
	int	i;
	//printf("enter word\n");
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
	rec2(data, c->right, start, end - 1);
}

void	rec2(t_data *data, t_ast *c, int start, int end)
{
	//printf("rec | start = %d, end = %d\n", start, end);
 	if (ft_lstchr(data->final_lex, start, end, PIPE) != - 1)
		fillnode_pipe(data, c, start, end);
	else if (ft_lstchr(data->final_lex, start, end, R_REDIR) != - 1)
		fillnode_redir(data, c, start, end);
	else if (ft_lstchr(data->final_lex, start, end, WORD) != -1 )
		fillnode_word(data, c, start, end);
	return ;
}

void ast(t_data *data)
{
	t_ast	*c;

	data->ast = (t_ast **)malloc(sizeof(t_ast *));
	if (!data->ast)
		reloop(data, "c->ast2: Allocation Failure (ast2)");
	c = (t_ast *)malloc(sizeof(t_ast));
	if (!c)
		reloop(data, "c: Allocation Failure (ast2)");
	*(data->ast) = c;
	rec2(data, c, 0, ft_lstsize(*(data->final_lex)));
}
