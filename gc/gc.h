#ifndef GC_H
# define GC_H

# include <stdlib.h>

typedef struct s_alloc
{
	void			*ptr;
	struct s_alloc	*next;
}	t_alloc;

typedef struct s_gc
{
	t_alloc	*head;
}	t_gc;

void	*gc_malloc(t_gc *gc, size_t size);
void	gc_add(t_gc *gc, void *ptr);
void	gc_clear(t_gc *gc);

#endif