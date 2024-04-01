/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:24:25 by bbialy            #+#    #+#             */
/*   Updated: 2024/03/31 23:57:33 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	data_init(t_data *data)
{
	data->input = NULL;
	data->lex = NULL;
	data->final_lex = NULL;
	data->ast = NULL;
	data->exit_status = 0;
	data->actual_pid = 74749;
}
