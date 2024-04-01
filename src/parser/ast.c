#include "minishell.h"

void	rec2(t_data *data, t_ast *c, int start, int end);

static void	show_tree(t_ast *tree, int n)
{
	int i;

	i = n;

	if (tree->right)
		show_tree(tree->right, n + 1);
	while (i-- > 0)
		printf("\t");
	if (tree)
	{
		if (tree->token == PIPE)
			printf("\033[30;42m");
		else if (tree->token == LL_REDIR)
			printf("\033[30;46m");
		else if (tree->token == R_REDIR || tree->token == RR_REDIR || tree->token == L_REDIR)
			printf("\033[30;45m");
		else if (tree->token == WORD)
			printf("\033[30;47m");
		printf("[%d] %s ", n, tree->str);
		printf("\033[0m");
		printf("\n");
	}
	if (tree->left)
		show_tree(tree->left, n + 1);
}

static int	ft_lstchr(t_list **lex, int start, int end, int token)
{
	t_list	*c;
	int	i;

	c = ft_lstat(lex, start);
	i = 0;
	printf("enter lstchr\n");
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

void	fillnode_pipe(t_data *data, t_ast *c, int start, int end)
{
	int	i;

	printf("enter pipe\n");
	i = ft_lstchr(data->final_lex, start, end, PIPE); // STRCHR LAST PIPE
	c->token = ((t_final *)ft_lstat(data->final_lex, i)->content)->token;
	c->str = ((t_final *)ft_lstat(data->final_lex, i)->content)->str;
	c->right = NULL;
	c->left = NULL;
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

	printf("enter redir\n");
	i = ft_lstchr(data->final_lex, start, end, R_REDIR);
	c->token = ((t_final *)ft_lstat(data->final_lex, i)->content)->token;
	c->str = ((t_final *)ft_lstat(data->final_lex, i)->content)->str;
	c->right = NULL;
	c->left = NULL;
	c->left = (t_ast *)malloc(sizeof(t_ast));
	if (!c->left)
		reloop(data, "c->left: Allocation Failure (fillnode_redir)");
	c->left->token = ((t_final *)ft_lstat(data->final_lex, i)->next->content)->token;
	c->left->str = ((t_final *)ft_lstat(data->final_lex, i)->next->content)->str;
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
	rec2(data, c->right, start, end - 2);
}

void	fillnode_word(t_data *data, t_ast *c, int start, int end)
{
	int	i;
	printf("enter word\n");
	i = ft_lstchr(data->final_lex, start, end, WORD);
	c->token = ((t_final *)ft_lstat(data->final_lex, i)->content)->token;
	c->str = ((t_final *)ft_lstat(data->final_lex, i)->content)->str;
	c->right = NULL;
	c->left = NULL;
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
	printf("rec | start = %d, end = %d\n", start, end);
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
	printf("TEST\n");
	show_tree(c, 0);
}
