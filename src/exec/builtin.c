/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:29:10 by glemaire          #+#    #+#             */
/*   Updated: 2024/04/21 19:16:31 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_builtin(t_data *data, t_ast *c)
{
	t_ast	*temp;

	temp = c;
	while (temp)
	{
		if (temp->token == WORD)
			break ;
		temp = temp->right;
	}
	if (!temp)
		return ;
	if (!ft_strcmp(temp->str, "echo"))
		builtin_echo(data, temp);
/* 	else if (!ft_strcmp(temp->str, "cd"))
		builtin_cd(data, temp);
	else if (!ft_strcmp(temp->str, "pwd"))
		builtin_pwd(data, temp);
	else if (!ft_strcmp(temp->str, "export"))
		builtin_export(data, temp);
	else if (!ft_strcmp(temp->str, "unset"))
		builtin_unset(data, temp);
	else if (!ft_strcmp(temp->str, "env"))
		builtin_env(data, temp); */
}
