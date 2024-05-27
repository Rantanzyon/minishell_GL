/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:06:54 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/27 11:23:57 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/include/libft.h"
# include "minishell_errors.h"
# include "colors.h"
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <sys/wait.h>

/* int execve_success;
void sig_handler(int signum); */

enum e_Token
{
	PIPE,
	WORD,
	FILENAME,
	L_REDIR,
	R_REDIR,
	RR_REDIR,
	LL_REDIR,
};

enum e_Pretok
{
	REDIR_LEFT,
	REDIR_RIGHT,
	EXP,
	DQ,
	SQ,
	CHAR,
	OSEF,
	ESPACE,
	EMPTY_STR,
	AND,
	OR,
	PAR_L,
	PAR_R,
	START
};

enum e_Builtin
{
	FIRST_ROW,
	BUILTIN_EXIT,
	BUILTIN_CD
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
	int				hdfd;
	int				prev_node;
	t_list			**lst;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_data
{
	char	*input;
	char	**argv;
	char	**envp;
	t_list	**env;
	t_list	**lex;
	t_list	**temp_final_lex;
	t_list	**final_lex;
	t_ast	**ast;
	int		actual_pid;
	char	**args;
	char	**path;
	int		exit;
	int		in;
	int		out;
	int		pipelvl;
}	t_data;

void	prompt(t_data *data);
t_data	*data_creation(char **argv, char **envp);
void	data_init(t_data *data);
void	ft_loop(t_data *data);
void	reloop(t_data *data, int status, char *name, char *err);
void	data_destroy(t_data *data, int status, char *name, char *err);
void	data_destroy_exit(t_data *data, int status, char *name, char *err);
void	err_message(t_data *data, int status, char *name, char *err);
void	free_final_lex(t_list **a);
void	free_env(t_list **a);
void	free_ast(t_ast *c);
void	ft_free_array(char **arr);

void	lexer(t_data *data);
void	lexer_quote(t_data *data);
void	lexer_expand(t_data *data);
t_list	*exp_var(t_data *data, t_list *cursor);
void	ft_lstadd_str(t_data *data, char *name, int n);
void	lexer_quote_final(t_data *data);
void	lexer_temp(t_data *data);
void	lexer_final(t_data *data);
void	ft_add_token(t_data *data, t_list **lst, char *word, int token);
void	is_redir_right(t_data *data, t_list **cursor);
void	is_redir_left(t_data *data, t_list **cursor);
void	is_word(t_data *data, t_list **cursor);
void	is_empty(t_data *data, t_list **cursor);
void	is_or(t_data *data, t_list **cursor);
void	is_and(t_data *data, t_list **cursor);
void	is_par_l(t_data *data, t_list **cursor);
void	is_par_r(t_data *data, t_list **cursor);

void	parser(t_data *data);
void	syntax_check(t_data *data);
void	syntax1(t_data *data, t_list *c);
void	syntax2(t_data *data, t_list *c);
void	syntax3(t_data *data, t_list *c);
void	syntax4(t_data *data, t_list *c);
void	syntax5(t_data *data, t_list *c);
void	syntax_error(t_data *data, char *str);
void	rename_tok(t_data *data);

void	ast(t_data *data);
void	rec(t_data *data, t_ast *c, int start, int end);
t_ast	*fill_node(t_data *data, t_ast *c, int i);
int		ft_findlast_pipe(t_list **lex, int start, int end);
int		ft_findlast_andor(t_list **lex, int start, int end);
int		ft_first_parenthesis(t_list **lex, int start);

void	exec(t_data *data);
void	heredoc(t_data *data, t_ast *c);
void	executer(t_data *data, t_ast *c, int in, int out);
void	exec_pipe(t_data *data, t_ast *c);
void	exec_and(t_data *data, t_ast *c);
void	exec_or(t_data *data, t_ast *c);
void	exec_expr(t_data *data, t_ast *c);
void	update_redir(t_data *data, t_ast *c);
void	exec_cmd(t_data *data, t_ast *c);
void	do_execve(t_data *data);
void	close_in(t_data *data);
void	close_out(t_data *data);

int		is_builtin(t_ast *c);
void	builtin(t_data *data, t_ast *c);
void	special_builtin(t_data *data, t_ast *c);
void	builtin_echo(t_data *data, t_ast *c);
void	builtin_exit(t_data *data, t_ast *c);

void	print_lex(t_data *data);
void	print_lst(t_list **lst);
void	print_ast(t_ast *tree, int n);

#endif