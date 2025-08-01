#ifndef PARSER_H
# define PARSER_H

// Represents a Redirection
// example: echo hi > out.txt < in.txt
// [
//   { type: T_REDIR_OUT, file: "out.txt" },
//   { type: T_REDIR_IN,  file: "in.txt"  }
// ]
typedef struct	s_redir
{
	t_token_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

// Represents a Simple Command (No Operators)
// example: ls -l > out.txt
// argv = ["ls", "-l", NULL]
// redirs = [ {type: T_REDIR_OUT, file: "out.txt"} ]
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

// AST Node Representing Any Shell Command
// example: ls | grep x && echo ok
// t_cmd_node
// ├─ type = N_AND
// ├─ left = t_cmd_node (PIPE)
// │    ├─ left = "ls"
// │    └─ right = "grep x"
// ├─ right = "echo ok"
typedef struct s_cmd_node
{
	t_node_type			type;
	struct s_cmd_node	*left;
	struct s_cmd_node	*right;
	t_cmd				*cmd;
}	t_cmd_node;

t_cmd_node	*parse_input(t_token *tokens, t_minishell *sh);
t_cmd_node	*parse_simple_command(t_token **curr, t_minishell *sh);

t_redir	*collect_redirs(t_token **curr, t_minishell *sh);
t_redir	*parse_redirection(t_token **curr, t_minishell *sh);
size_t	count_args(t_token *token);
char	**gather_args(t_token **token_ptr, size_t argc, t_minishell *sh);

#endif