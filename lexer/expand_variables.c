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

static size_t	handle_backtick_expansion(const char *input, size_t i,
					t_expand_ctx *ctx)
{
	const char	*cmd_start;
	size_t		cmd_len;
	char		*cmd;

	i++;
	cmd_start = &input[i];
	cmd_len = 0;
	while (input[i] && input[i] != '`')
	{
		cmd_len++;
		i++;
	}
	if (input[i] == '`')
	{
		cmd = gc_malloc(&ctx->sh->gc, cmd_len + 1);
		ft_strncpy(cmd, cmd_start, cmd_len);
		cmd[cmd_len] = '\0';
		append_command_output(cmd, ctx->res, ctx->j, ctx->sh);
		return (i + 1);
	}
	ctx->res[(*ctx->j)++] = '`';
	while (cmd_len-- > 0)
		ctx->res[(*ctx->j)++] = *cmd_start++;
	return (i);
}

static size_t	process_expansion_char(const char *input, size_t i,
					t_expand_ctx *ctx)
{
	if (input[i] == '$' && input[i + 1] && is_var_char(input[i + 1], 1))
		return (handle_variable_expansion(input, i, ctx));
	if (input[i] == '`')
		return (handle_backtick_expansion(input, i, ctx));
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
