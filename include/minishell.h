/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:06:54 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/03 22:57:07 by glemaire         ###   ########.fr       */
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
	WORD,
	FILENAME,
	L_REDIR,
	R_REDIR,
	RR_REDIR,
	LL_REDIR
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
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_exec
{
	int	fd_in;
	int	fd_out;
	int	here_doc;
	int	exit_status;
	
}	t_exec;

typedef struct s_data
{
	char	*input;
	char	**argv;
	char	**envp;
	t_list	**env;
	t_list	**lex;
	t_list	**final_lex;
	t_ast	**ast;
	t_exec	*exec;
	int		exit_status;
	int		actual_pid;
	int		here_doc;
	
}	t_data;

void	prompt(t_data *data);
t_data	*data_creation(char **argv, char **envp);
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
void	rec(t_data *data, t_ast *c, int start, int end);
int		ft_lstchr(t_list **lex, int start, int end, int token);
int		ft_findlast(t_list **lex, int start, int end, int token);
t_ast	*fill_node(t_data *data, t_ast *c, int i);

void	exec(t_data *data, t_ast *c, int in, int out);

void	print_lex(t_data *data);
void	print_lst(t_list **lst);
void	print_ast(t_ast *tree, int n);
void	print_ast2(t_data *data);

#endif