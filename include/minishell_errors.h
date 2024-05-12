/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_errors.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 09:35:56 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/27 15:02:40 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_ERRORS_H
# define MINISHELL_ERRORS_H

# define CMD_NF 127

# define SYNTX_ERR_PAR "syntax error: Unclosed parenthesis"
# define SYNTX_ERR_QUOTE "syntax error: Unclosed quote"
# define SYNTX_ERR_NL "syntax error near unexpected token `newline'"
# define SYNTX_ERR_PIPE "syntax error near unexpected token `|'"
# define SYNTX_ERR_R "syntax error near unexpected token `>'"
# define SYNTX_ERR_RR "syntax error near unexpected token `>>'"
# define SYNTX_ERR_L "syntax error near unexpected token `<'"
# define SYNTX_ERR_LL "syntax error near unexpected token `<<'"
# define SYNTX_ERR_PARL "syntax error near unexpected token `('"
# define SYNTX_ERR_PARR "syntax error near unexpected token `)'"

# define CMDNF "command not found"
# define NUMREQ "numeric argument required"
# define TOOMANYARG "too many arguments"

#endif