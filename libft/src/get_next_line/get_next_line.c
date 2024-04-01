/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glemaire <glemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 15:00:29 by glemaire          #+#    #+#             */
/*   Updated: 2024/03/30 06:15:52 by glemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_reducebuffer(char *buff, char *line)
{
	int		i;
	size_t	j;
	char	*newline;

	i = 0;
	while (line[i] != '\n')
		i++;
	j = 0;
	while (line[j + i + 1])
	{
		buff[j] = line[j + i + 1];
		j++;
	}
	while (j < BUFFER_SIZE)
		buff[j++] = '\0';
	newline = ft_substr(line, 0, i + 1);
	if (!newline)
		return (free(line), NULL);
	free(line);
	return (newline);
}

static char	*complete_line(char *buff, char *line, int fd)
{
	ssize_t	rd;

	rd = 1;
	while (!ft_strchr(buff, '\n'))
	{
		ft_bzero(buff, BUFFER_SIZE);
		rd = read(fd, buff, BUFFER_SIZE);
		if (rd == -1)
			return (free(line), NULL);
		if (rd == 0)
			break ;
		line = gnl_strjoin(line, buff);
		if (!line)
			return (NULL);
	}
	if (rd > 0)
		line = ft_reducebuffer(buff, line);
	return (line);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	buff[BUFFER_SIZE + 1];

	if (fd < 0)
		return (NULL);
	buff[BUFFER_SIZE] = '\0';
	line = ft_strdup(buff);
	if (!line)
		return (NULL);
	if (ft_strchr(buff, '\n'))
		return (ft_reducebuffer(buff, line));
	line = complete_line(buff, line, fd);
	if (line && ft_strlen(line) == 0)
		return (free(line), NULL);
	return (line);
}
