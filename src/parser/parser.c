/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:23:24 by bbialy            #+#    #+#             */
/*   Updated: 2024/04/02 01:26:43 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parser(t_data *data)
{
	syntax_check(data);
	rename_tok(data);
	//print_lst(data->final_lex);
	ast(data);
	print_ast(*data->ast, 0);
}