/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eates <eates@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 12:28:54 by oakbulak          #+#    #+#             */
/*   Updated: 2024/11/15 20:01:09 by eates            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static size_t	sl(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}

static char	*ft_strdup(const char *s1, size_t limit)
{
	char	*res;
	size_t	i;

	res = malloc(limit + 1);
	if (!res)
		return (NULL);
	i = -1;
	while (++i < limit)
		res[i] = s1[i];
	return (res[i] = '\0', res);
}

static void	*special_free(char **stash, int create_line)
{
	char	*line;

	if (!*stash)
		return (NULL);
	if (create_line == 0)
		return (free(*stash), *stash = NULL);
	if (create_line == 1)
		return (line = ft_strdup(*stash, sl(*stash)),
			free(*stash), *stash = NULL, line);
	return (NULL);
}

static int	ctrl(char **stash, char **line, size_t i, char *tmp)
{
	char	*res;

	if (!*stash)
		res = ft_strdup(tmp, sl(tmp));
	else
		res = malloc(sl(*stash) + sl(tmp) + 1);
	if (!res)
		return (special_free(stash, 0), *line = NULL, 1);
	if (*stash)
	{
		while ((*stash)[++i])
			res[i] = (*stash)[i];
		while (*tmp)
			res[i++] = *tmp++;
		res[i] = '\0';
	}
	special_free(stash, (i = 0));
	*stash = res;
	while ((*stash)[i] && (*stash)[i] != '\n')
		i++;
	if (!(*stash)[i])
		return (0);
	return (*line = ft_strdup(*stash, i + 1), 1);
}

char	*get_next_line(int fd)
{
	static char	*stash[10240];
	char		buf[BUFFER_SIZE + 1];
	char		*line;
	char		*tmp;
	long		ret;

	if (fd == -1)
		return (NULL);
	ret = BUFFER_SIZE;
	while (ret > 0)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if (ret == -1 || (ret == 0 && !stash[fd]))
			return (special_free(&stash[fd], 0));
		buf[ret] = '\0';
		if (!ctrl(&stash[fd], &line, -1, buf))
			continue ;
		if (!line)
			return (special_free(&stash[fd], 0));
		tmp = NULL;
		if (stash[fd][sl(line)])
			tmp = ft_strdup(stash[fd] + sl(line), sl(stash[fd]) - sl(line));
		return (special_free(&stash[fd], 0), stash[fd] = tmp, line);
	}
	return (special_free(&stash[fd], 1));
}
