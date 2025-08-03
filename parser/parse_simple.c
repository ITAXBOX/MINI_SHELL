#include "minishell.h"

char	**read_and_collect_matches(DIR *dir, const char *pattern
		, t_gc *gc, int *out_count)
{
	struct dirent	*entry;
	char			**matches;
	int				count;
	int				capacity;

	capacity = 16;
	matches = gc_malloc(gc, sizeof(char *) * (capacity + 1));
	count = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && match_pattern(pattern, entry->d_name))
		{
			if (count >= capacity)
			{
				capacity *= 2;
				matches = copy_match_list(matches, count, capacity, gc);
			}
			matches[count++] = gc_strdup(entry->d_name, gc);
		}
		entry = readdir(dir);
	}
	matches[count] = NULL;
	*out_count = count;
	return (matches);
}

static t_redir	*parse_redirection(t_token **curr, t_minishell *sh)
{
	t_redir	*redir;

	if (!*curr || (*curr)->type < T_REDIR_IN || (*curr)->type > T_HEREDOC)
		return (NULL);
	if (!(*curr)->next || (*curr)->next->type != T_WORD)
		return (NULL);
	redir = gc_malloc(&sh->gc, sizeof(t_redir));
	redir->type = (*curr)->type;
	redir->file = (*curr)->next->value;
	redir->next = NULL;
	*curr = (*curr)->next->next;
	return (redir);
}

static t_redir	*collect_redirs(t_token **curr, t_minishell *sh)
{
	t_redir	*head;
	t_redir	*tail;
	t_redir	*new;

	head = NULL;
	tail = NULL;
	while (*curr && ((*curr)->type >= T_REDIR_IN && (*curr)->type <= T_HEREDOC))
	{
		new = parse_redirection(curr, sh);
		if (!new)
			return (NULL);
		if (!head)
			head = new;
		else
			tail->next = new;
		tail = new;
	}
	return (head);
}

t_cmd_node	*parse_simple_command(t_token **curr, t_minishell *sh)
{
	t_cmd_node	*node;
	t_cmd		*cmd;
	size_t		argc;

	if (!*curr || (*curr)->type != T_WORD)
		return (NULL);
	cmd = gc_malloc(&sh->gc, sizeof(t_cmd));
	argc = count_args(*curr);
	cmd->argv = gather_args(curr, argc, sh);
	cmd->redirs = collect_redirs(curr, sh);
	node = gc_malloc(&sh->gc, sizeof(t_cmd_node));
	node->type = N_SIMPLE;
	node->cmd = cmd;
	node->left = NULL;
	node->right = NULL;
	return (node);
}
