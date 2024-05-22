/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:26:41 by bbialy            #+#    #+#             */
/*   Updated: 2024/05/22 20:54:52 by glemaire         ###   ########.fr       */
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
		if (((t_lex *)cursor->content)->pretok == REDIR_LEFT)
			pretok_str = "REDIR_LEFT";
		else if (((t_lex *)cursor->content)->pretok == REDIR_RIGHT)
			pretok_str = "REDIR_RIGHT";
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
		else if (((t_lex *)cursor->content)->pretok == EMPTY_STR)
			pretok_str = "EMPTY_STR";
		else if (((t_lex *)cursor->content)->pretok == AND)
			pretok_str = "AND";
		else if (((t_lex *)cursor->content)->pretok == OR)
			pretok_str = "OR";
		else if (((t_lex *)cursor->content)->pretok == PAR_L)
			pretok_str = "PAR_L";
		else if (((t_lex *)cursor->content)->pretok == PAR_R)
			pretok_str = "PAR_R";
		else
			pretok_str = "ESPACE";
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
		printf("%s ",((t_final *)cursor->content)->str);
		cursor = cursor->next;
	}
	printf("\n");
}

void	print_ast(t_ast *tree, int n)
{
	int i;

	i = n;
	if (tree->right)
		print_ast(tree->right, n + 1);
	while (i-- > 0)
		printf("\t");
	if (tree)
	{
		if (tree->token == PIPE)
			printf("\033[30;42m");
		else if (tree->token == R_REDIR || tree->token == RR_REDIR || tree->token == L_REDIR || tree->token == LL_REDIR)
			printf("\033[30;45m");
		else if (tree->token == WORD)
			printf("\033[30;47m");
		else if (tree->token == FILENAME)
			printf("\033[30;43m");
		else if (tree->token == AND || tree->token == OR)
			printf("\033[30;41m");
		else if (tree->token == PAR_L || tree->token == PAR_R)
			printf("\033[30;44m");
		printf("[%d] %s ", tree->prev_node, tree->str);
		printf("\033[0m");
		printf("\n");
	}
	if (tree->left)
		print_ast(tree->left, n + 1);
}
