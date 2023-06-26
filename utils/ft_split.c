/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysabr <ysabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 20:31:49 by ysabr             #+#    #+#             */
/*   Updated: 2023/06/11 11:00:10 by ysabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	**ft_free(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
	return (NULL);
}

static int	count_word(const char *s, int c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

static char	**allocate_word(const char *str, char c)
{
	int		count;
	char	**subword;

	if (!str)
		return (NULL);
	count = count_word(str, c);
	subword = (char **)ft_calloc((count + 1), sizeof(char *));
	if (!subword)
		return (NULL);
	return (subword);
}

char	**ft_split(char const *s, char c)
{
	int		size;
	char	**split;
	int		i;
	int		j;

	i = -1;
	split = allocate_word(s, c);
	if (!split)
		return (NULL);
	j = 0;
	while (++i < count_word(s, c))
	{
		size = 0;
		while (s[j] == c)
			j++;
		while (s[j + size] != c && s[j + size])
			size++;
		split[i] = (char *)ft_calloc(size + 1, sizeof(char));
		if (!split[i])
			return (ft_free(split));
		ft_strlcpy(split[i], &s[j], size + 1);
		j += size;
	}
	return (split);
}
