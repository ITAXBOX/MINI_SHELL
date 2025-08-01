#ifndef UTILS_H
# define UTILS_H

char	*ft_strchr(const char *s, int c);
char	*ft_strncpy(char *dest, const char *src, unsigned int n);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
size_t	ft_strlen(const char *s);

int		ft_atoi(const char *str);
char	**copy_envp(char **envp, t_gc *gc);
int		env_find_index(char **envp, const char *key);
void	env_set(char ***envp_ptr, const char *entry, t_gc *gc);

int		validate_token_stream(t_token *token, t_minishell *sh);

#endif