#ifndef LEXER_H
# define LEXER_H

typedef enum e_token_type
{
    T_WORD,
    T_PIPE,
    T_REDIR_IN,
    T_REDIR_OUT,
    T_REDIR_APPEND,
    T_HEREDOC,
    T_AND_IF,
    T_OR_IF,
    T_PAREN_L,
    T_PAREN_R
}	t_token_type;

typedef struct s_token
{
    char			*value;
    t_token_type	type;
    struct s_token	*next;
}	t_token;

typedef struct s_minishell t_minishell;

t_token *tokenize_input(const char *input, t_minishell *sh);

void	skip_whitespace(const char **s);
char	*extract_word(const char **s, t_minishell *sh);
char	*extract_operator(const char **s, t_token_type *type, t_minishell *sh);

char	*get_var_value(const char *var, t_minishell *sh);
char	*extract_quoted_word(const char **s, t_minishell *sh);

char	*expand_variables(const char *input, t_minishell *sh);

#endif