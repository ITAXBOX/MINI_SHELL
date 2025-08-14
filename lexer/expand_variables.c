#include "minishell.h"

static size_t	handle_variable_expansion(const char *input, size_t i,
					t_expand_ctx *ctx)
{
	char	*name;

	i++;
	name = extract_var_name(input, &i, ctx->sh);
	append_var_value(name, ctx->res, ctx->j, ctx->sh);
	return (i);
}

static size_t	process_expansion_char(const char *input, size_t i,
					t_expand_ctx *ctx)
{
	if (input[i] == '$' && input[i + 1] && is_var_char(input[i + 1], 1))
		return (handle_variable_expansion(input, i, ctx));
	ctx->res[(*ctx->j)++] = input[i++];
	return (i);
}

char	*expand_variables(const char *input, t_minishell *sh)
{
	char			*res;
	size_t			i;
	size_t			j;
	t_expand_ctx	ctx;

	if (!input)
		return (gc_strdup("", &sh->gc));
	res = gc_malloc(&sh->gc, calculate_needed_size(input, sh));
	if (!res)
		return (gc_strdup("", &sh->gc));
	i = 0;
	j = 0;
	ctx.res = res;
	ctx.j = &j;
	ctx.sh = sh;
	while (input[i])
		i = process_expansion_char(input, i, &ctx);
	res[j] = '\0';
	return (res);
}
