/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:24:36 by bbialy            #+#    #+#             */
/*   Updated: 2024/03/30 16:41:38 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_slash(char *pwd)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (pwd[i])
	{
		if (pwd[i] == '/')
			count++;
		i++;
	}
	return (count);
}

static char	*pwd_shorter(char *pwd, t_data *data)
{
	char	*new;
	int		count;
	size_t	len;

	count = 0;
	len = ft_strlen(pwd);
	while (count != 2)
	{
		if (pwd[len] == '/')
			count++;
		len--;
	}
	new = ft_substr(pwd, len + 1, ft_strlen(pwd) - len);
	if (!new)
		reloop(data, "new : Allocation failure (pwd_shorter)");
	return (new);
}

static char	*pwd_resize(t_data *data)
{
	char	*pwd;
	char	*new;
	int		count;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		reloop(data, "pwd : Allocation failure (pwd_resize)");
	count = count_slash(pwd);
	if (count > 2)
		new = pwd_shorter(pwd, data);
	else
	{
		new = ft_strdup(pwd);
		if (!new)
			reloop(data, "new : Allocation failure (pwd_resize)");
	}
	free(pwd);
	new = gnl_strjoin(new, "$ \033[0m");
	if (!new)
		reloop(data, "new : Allocation failure (pwd_resize)");
	return (new);
}

void	prompt(t_data *data)
{
	char	*pwd;
	char	*pwd_colored;

	pwd = pwd_resize(data);
	pwd_colored = ft_strjoin("\033[34m", pwd);
	free(pwd);
	if (!pwd_colored)
		reloop(data, "pwd_colored : Allocation failure (prompt)");
	data->input = readline(pwd_colored);
	free(pwd_colored);
	if (!data->input || !ft_strcmp(data->input, "exit"))
		data_destroy_exit(data, EXIT_SUCCESS, NULL);
	if (ft_strlen(data->input) != 0)
		add_history(data->input);
}
