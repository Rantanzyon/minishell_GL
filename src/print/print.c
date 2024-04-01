/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:26:41 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/01 17:45:53 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_lex(t_data *data)
{
	t_list	*cursor;
	char	*pretok_str;

	cursor = *data->lex;
	while (cursor)
	{
		if (((t_lex *)cursor->content)->pretok == SYM)
			pretok_str = "SYM";
		else if (((t_lex *)cursor->content)->pretok == EXP)
			pretok_str = "EXP";
		else if (((t_lex *)cursor->content)->pretok == DQ)
			pretok_str = "DQ";
		else if (((t_lex *)cursor->content)->pretok == SQ)
			pretok_str = "SQ";
		else if (((t_lex *)cursor->content)->pretok == CHAR)
			pretok_str = "CHAR";
		else if (((t_lex *)cursor->content)->pretok == OSEF)
			pretok_str = "OSEF";
		else
			pretok_str = "SPACE";
		printf("%c: %s\n", ((t_lex *)cursor->content)->c, pretok_str);
		cursor = cursor->next;
	}
	printf("---------------------\n");
}

void	print_lst(t_list **lst)
{
	t_list	*cursor;

	if (!lst || !(*lst))
		return ;
	cursor = *lst;
	while (cursor)
	{
		printf("[%d]%s ", ((t_final *)cursor->content)->token, ((t_final *)cursor->content)->str);
		cursor = cursor->next;
	}
	printf("\n");
}

void	print_ast(t_ast *tree, int n)
{
	int i;

	i = n;
	//printf("PRINT AST\n");
	if (tree->right)
	{
		//printf("%d\n", n);
		print_ast(tree->right, n + 1);
	}
	//printf("AST2\n");
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
		else if (tree->token == FILENAME)
			printf("\033[30;43m");
		printf("[%d] %s ", n, tree->str);
		printf("\033[0m");
		printf("\n");
	}
	if (tree->left)
	{
		//printf("%d\n", n);	
		print_ast(tree->left, n + 1);
	}
}
