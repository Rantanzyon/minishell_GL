/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:06:54 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/01 19:57:16 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/include/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>

enum e_Token
{
	PIPE,
	L_REDIR,
	R_REDIR,
	RR_REDIR,
	LL_REDIR,
	WORD,
	FILENAME
};

enum e_Pretok
{
	SYM,
	EXP,
	DQ,
	SQ,
	CHAR,
	OSEF,
	ESPACE
};

typedef struct s_lex
{
	int				pretok;
	char			c;
}	t_lex;

typedef struct s_final
{
	int		token;
	char	*str;
}	t_final;

typedef struct s_ast
{
	char			*str;
	int				token;
	// struct s_final *node;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_data
{
	char	*input;
	char	**envp;
	t_list	**lex;
	t_list	**final_lex;
	t_ast	**ast;
	int		exit_status;
	int		actual_pid;
	int		here_doc;
}	t_data;

void	prompt(t_data *data);
t_data	*data_creation(char **envp);
void	data_init(t_data *data);
void	ft_loop(t_data *data);
void	reloop(t_data *data, char *err);
void	data_destroy(t_data *data, char *err);
void	data_destroy_exit(t_data *data, int status, char *err);

void	lexer(t_data *data);
void	lexer_quote(t_data *data);
void	lexer_expand(t_data *data);
t_list	*exp_var(t_data *data, t_list *cursor);
void	ft_lstadd_str(t_data *data, char *name, int n);
void	lexer_quote_final(t_data *data);
void	lexer_final(t_data *data);
void	ft_add_token(t_data *data, char *word, int token);
void	remove_osef(t_data *data);

void	parser(t_data *data);
void	syntax_check(t_data *data);
void	rename_tok(t_data *data);
void	ast(t_data *data);

void	print_lex(t_data *data);
void	print_lst(t_list **lst);
void	print_ast(t_ast *tree, int n);
void	print_ast2(t_data *data);

#endif