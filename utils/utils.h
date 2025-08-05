#ifndef UTILS_H
# define UTILS_H

typedef struct s_redir	t_redir;

char	*ft_strchr(const char *s, int c);
char	*ft_strncpy(char *dest, const char *src, unsigned int n);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
size_t	ft_strlen(const char *s);

int		ft_atoi(const char *str);
char	**copy_envp(char **envp);
int		env_find_index(char **envp, const char *key);
void	env_set(char ***envp_ptr, const char *entry);

size_t	count_args(t_token *token);
void	free_envp(char **envp);
char	*ft_strdup(const char *s);
int		is_numeric(const char *str);
int		ft_strcmp(const char *s1, const char *s2);

void	print_sorted_env_export(char **envp, t_gc *gc);
int		validate_token_stream(t_token *token, t_minishell *sh);

int		is_only_whitespace(const char *s);
char	**resize_argv(char **old_argv, size_t old_size,
			size_t new_capacity, t_minishell *sh);
int		match_pattern(const char *pattern, const char *text);
void	syntax_error(const char *token, t_minishell *sh);
int		is_valid_identifier(const char *s);

int		ft_isalpha(int c);
int		ft_isalnum(int c);

#endif