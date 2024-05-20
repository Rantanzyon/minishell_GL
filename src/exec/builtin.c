/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:29:10 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/20 11:53:13 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_builtin(t_ast *c)
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
		return (0);
	if (!ft_strcmp(temp->str, "echo"))
		return (1);
	else if (!ft_strcmp(temp->str, "exit"))
		return (1);
	return (0);
}

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
	else if (!ft_strcmp(temp->str, "exit"))
		builtin_exit(data, temp);
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
