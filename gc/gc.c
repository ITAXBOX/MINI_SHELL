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
