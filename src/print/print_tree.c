/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 18:45:05 by bbialy            #+#    #+#             */
/*   Updated: 2024/03/31 23:50:37 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define NODE 7
static int stage;
typedef struct	s_tree
{
	int		token;
	char	c;
}	t_tree;

static int	count_etage(t_ast *ast, int etage, int k)
{
	if (etage > k)
		k = etage;
	if (ast->left)
		k = count_etage(ast->left, etage + 1, k);
	if (ast->right)
		k = count_etage(ast->right, etage + 1, k);
	return (k);
}

static int	pow_2(int n)
{
	int	i;
	int	nb;

	i = 0;
	nb = 1;
	while (i < n)
	{
		nb *= 2;
		i++;
	}
	return (nb);
}

static void	fill_node(t_ast *c, t_tree **tree, int i, int j)
{
	int	n;
	int	k;

	k = 0;
	if (ft_strlen(c->content) <= NODE)
		k = (NODE - ft_strlen(c->content)) / 2;
	n = 0;
	while (n < NODE && n < ft_strlen(c->content))
	{
		tree[i][j + n + k].c = c->content[n];
		n++;
	}
	n = 0;
	while (n < NODE)
	{
		tree[i][j + n].token = c->token;
		n++;
	}
}

static void	fill_leftlink(t_tree **tree, int i, int j, int old_j)
{
	tree[i + 1][j].c = '|';
	tree[i][j].c = '+';
	j++;
	while (j < old_j)
	{
		tree[i][j].c = '-';
		j++;
	}
}

static void	fill_rightlink(t_tree **tree, int i, int j, int old_j)
{
	while (old_j < j)
	{
		tree[i][old_j].c = '-';
		old_j++;
	}
	tree[i + 1][j].c = '|';
	tree[i][j].c = '+';
}

static void	rec(t_ast *c, t_tree **tree, int i, int j)
{
	int	n;

	n = pow_2(stage - i - 2) * NODE;
	fill_node(c, tree, i * 2, j);
	if (c->left)
	{
		fill_leftlink(tree, i * 2, j - n + (NODE / 2), j);
		rec(c->left, tree, i + 1, j - n);
	}
	if (c->right)
	{
		fill_rightlink(tree, i * 2, j + n + (NODE / 2), j + NODE);
		rec(c->right, tree, i + 1, j + n);
	}
}

static void	print_tree(t_tree **tree, int h, int w)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	printf("\033[0;91m");
	while (j < w)
	{
		printf("=");
		j++;
	}
	printf("\n");
	j = 0;
	while (j < (w - 5) / 2)
	{
		printf("=");
		j++;
	}
	printf(" AST ");
	j = 0;
	while (j < (w - 5) / 2)
	{
		printf("=");
		j++;
	}
	printf("\n");
	j = 0;
	while (j < w)
	{
		printf("=");
		j++;
	}
	printf("\n");
	printf("\033[0m");
	printf("\033[30;42m");
	printf("  PIPE  ");
	printf("\033[30;46m");
	printf(" HERE_D ");
	printf("\033[0m");
	printf("\n");
	printf("\033[30;47m");
	printf("  WORD  ");
	printf("\033[30;45m");
	printf("  RDIR  ");
	printf("\033[0m");
	printf("\n\n");
	i = 0;
	while (i < h)
	{
		j = 0;
		while (j < w)
		{
			if (tree[i][j].token != -1)
			{
				if (tree[i][j].token == PIPE)
					printf("\033[30;42m");
				else if (tree[i][j].token == LL_REDIR)
					printf("\033[30;46m");
				else if (tree[i][j].token == R_REDIR
					|| tree[i][j].token == RR_REDIR
					|| tree[i][j].token == L_REDIR)
					printf("\033[30;45m");
				else if (tree[i][j].token == WORD)
					printf("\033[30;47m");
			}
			printf("%c", tree[i][j].c);
			printf("\033[0m");
			j++;
		}
		printf("\n");
		i++;
	}
	j = 0;
	printf("\n");
	printf("\033[0;91m");
	while (j < w)
	{
		printf("=");
		j++;
	}
	printf("\n");
	printf("\033[0m");
}

static t_tree	**create_2d_struct(int h, int w)
{
	int		i;
	int		j;
	t_tree	**tree;

	tree = (t_tree **)malloc(h * sizeof(t_tree *));
	i = 0;
	while (i < h)
	{
		tree[i] = (t_tree *)malloc(w * sizeof(t_tree));
		j = 0;
		while (j < w)
		{
			tree[i][j].c = ' ';
			tree[i][j].token = -1;
			j++;
		}
		i++;
	}
	return (tree);
}

void	print_ast2(t_data *data)
{
	t_ast	*cur;
	int		width;
	t_tree	**tree;

	cur = *(data->ast);
	stage = count_etage(cur, 1, 0);
	width = (pow_2(stage - 1) * 2 - 1) * NODE;
	tree = create_2d_struct(stage * 2 - 1, width);
	rec(cur, tree, 0, (width - NODE) / 2);
	print_tree(tree, stage * 2 - 1, width);
}
