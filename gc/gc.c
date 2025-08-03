#include "minishell.h"

void	*gc_malloc(t_gc *gc, size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	gc_add(gc, ptr);
	return (ptr);
}

void	gc_add(t_gc *gc, void *ptr)
{
	t_alloc	*new;

	new = malloc(sizeof(t_alloc));
	if (!new)
		return ;
	new->ptr = ptr;
	new->next = gc->head;
	gc->head = new;
}

void	gc_clear(t_gc *gc)
{
	t_alloc	*tmp;

	while (gc->head)
	{
		tmp = gc->head;
		free(tmp->ptr);
		gc->head = tmp->next;
		free(tmp);
	}
}

char	*gc_strdup(const char *s, t_gc *gc)
{
	size_t	len;
	char	*dup;

	len = 0;
	while (s[len])
		len++;
	dup = gc_malloc(gc, len + 1);
	if (!dup)
		return (NULL);
	ft_strncpy(dup, s, len);
	dup[len] = '\0';
	return (dup);
}

char	*gc_strjoin(const char *s1, const char *s2, t_gc *gc)
{
	size_t	len1;
	size_t	len2;
	size_t	i;
	size_t	j;
	char	*res;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	res = gc_malloc(gc, len1 + len2 + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (i < len1)
	{
		res[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < len2)
	{
		res[i + j] = s2[j];
		j++;
	}
	res[i + j] = '\0';
	return (res);
}
