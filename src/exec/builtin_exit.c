/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 21:48:45 by glemaire          #+#    #+#             */
/*   Updated: 2024/05/25 09:41:19 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_ll(t_data *data, char *str, int i, int sign)
{
	int		len;
	int		j;
	char	*ref;

	len = 0;
	j = 0;
	if (sign == 1)
		ref = "9223372036854775807";
	else
		ref = "9223372036854775808";
	while (str[i + len] && ft_isdigit(str[i + len]))
		len++;
	if (len > 19)
		data_destroy_exit(data, 255, str, NUMREQ);
	else if (len == 19)
	{
		while (j < len)
		{
			if (str[i + j] > ref[j])
				data_destroy_exit(data, 255, str, NUMREQ);
			j++;
		}
	}
}

static long long	ft_compute_atoll(t_data *data, char *str, int i, int sign)
{
	long long	n;

	n = 0;
	while (str[i] && ft_isdigit(str[i]))
		n = n * 10 + str[i++] - '0';
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i])
		data_destroy_exit(data, 255, str, NUMREQ);
	if (sign == -1 && n % 256 != 0)
		return (256 - (n % 256));
	else if (sign == -1 && n % 256 == 0)
		return (0);
	return (n % 256);
}

int	analyse_arg_exit(t_data *data, char *str)
{
	int			i;
	long long	n;
	int			sign;

	i = 0;
	sign = 1;
	n = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-')
		sign = -1;
	else if (!ft_isdigit(str[i]) && str[i] != '+')
		data_destroy_exit(data, 255, str, NUMREQ);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] == '0')
		i++;
	check_ll(data, str, i, sign);
	n = ft_compute_atoll(data, str, i, sign);
	return (n);
}

void	builtin_exit(t_data *data, t_ast *c)
{
	int	n;
	t_ast *temp;

	temp = c;
	if (data->pipelvl == 0)
		ft_putendl_fd("exit", STDOUT_FILENO);
	while (temp->token != WORD)
		temp = temp->right;
	temp = temp->right;
	while (temp && temp->token != WORD)
		temp = temp->right;
	if (!temp)
		data_destroy_exit(data, EXIT_SUCCESS, NULL, NULL);
	n = analyse_arg_exit(data, temp->str);
	temp = temp->right;
	while (temp && temp->token != WORD)
		temp = temp->right;
	if (temp)
	{
		data->exit = EXIT_FAILURE;
		err_message(data, EXIT_FAILURE, "exit", TOOMANYARG);
		if (data->pipelvl > 0)
			data_destroy_exit(data, n, NULL, NULL);
	}
	else
		data_destroy_exit(data, n, NULL, NULL);
}

