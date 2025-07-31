#ifndef PARSER_H
# define PARSER_H

typedef struct	s_redir
{
	t_token_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct	s_cmd
{
	char	**argv;
	t_redir	*redirs;
}	t_cmd;

typedef enum	e_node_type
{
	N_SIMPLE,
	N_PIPE,
	N_AND,
	N_OR,
}	t_node_type;

typedef struct s_cmd_node
{
	t_node_type			type;
	struct s_cmd_node	*left;
	struct s_cmd_node	*right;
	t_cmd				*cmd;
}	t_cmd_node;

t_cmd_node	*parse_input(t_token *tokens, t_minishell *sh);
t_cmd_node	*parse_simple_command(t_token **curr, t_minishell *sh);

#endif