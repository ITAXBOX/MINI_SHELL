#include "minishell.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	temp;

	if (!s)
		return (NULL);
	i = 0;
	temp = (char)c;
	while (s[i] != '\0')
	{
		if (s[i] == temp)
			return ((char *)(s + i));
		i++;
	}
	if (temp == '\0')
		return ((char *)(s + i));
	return (NULL);
}

char	*ft_strncpy(char *dest, const char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while (i < n - 1 && s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*temp1;
	const unsigned char	*temp2;

	if (!dest && !src)
		return (NULL);
	i = 0;
	temp1 = (unsigned char *)dest;
	temp2 = (unsigned const char *)src;
	while (i < n)
	{
		temp1[i] = temp2[i];
		i++;
	}
	return (dest);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}
