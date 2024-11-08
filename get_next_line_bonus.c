/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakbulak <oakbulak@student.42kocaeli.com.  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 12:28:54 by oakbulak          #+#    #+#             */
/*   Updated: 2023/07/16 12:28:57 by oakbulak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*ft_copy_to_stash(char *stash, char *buf)
{
	char	*res;
	size_t	i;

	if (!stash)
		return (ft_strdup(buf));
	res = malloc(ft_strlen(stash) + ft_strlen(buf) + 1);
	if (!res)
		return (ft_free_stash(&stash, 0));
	i = -1;
	while (stash[++i])
		res[i] = stash[i];
	while (*buf)
		res[i++] = *buf++;
	return (res[i] = '\0', ft_free_stash(&stash, 0), res);
}

static int	ft_have_nl(char *s, char **line)
{
	size_t	i;
	char	c;

	if (!s)
		return (0);
	i = -1;
	while (s[++i])
		if (s[i] == '\n')
			return (c = s[++i], s[i] = '\0', *line = ft_strdup(s), s[i] = c, 1);
	return (0);
}

static char	*ft_recreate_stash(char *stash, size_t i)
{
	char	*res;

	if (!stash)
		return (NULL);
	if (stash[i + 1] == '\0')
		return (ft_free_stash(&stash, 0));
	res = ft_strdup(stash + i + 1);
	ft_free_stash(&stash, 0);
	return (res);
}

char	*get_next_line(int fd)
{
	char		*buf;
	long		ret;
	static char	*stash[5000];
	char		*line;

	ret = BUFFER_SIZE;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (ft_free_stash(&stash[fd], 0));
	buf = malloc(BUFFER_SIZE + 1);
	while (ret > 0)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if ((ret <= 0 && !stash[fd]) || ret == -1)
			return (free(buf), ft_free_stash(&stash[fd], 0));
		buf[ret] = '\0';
		stash[fd] = ft_copy_to_stash(stash[fd], buf);
		if (ft_have_nl(stash[fd], &line))
		{
			if (!line)
				return (free(buf), ft_free_stash(&stash[fd], 0));
			return (free(buf), stash[fd] = ft_recreate_stash(stash[fd],
					ft_strlen(line) - 1), line);
		}
	}
	return (free(buf), ft_free_stash(&stash[fd], 1));
}
