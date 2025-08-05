#include "minishell.h"

static int	check_overflow(long long result, char digit, int sign)
{
	if (sign == 1)
	{
		if (result > LLONG_MAX / 10
			|| (result == LLONG_MAX / 10 && (digit - '0') > LLONG_MAX % 10))
			return (1);
	}
	else
	{
		if (result > -(LLONG_MIN / 10)
			|| (result == -(LLONG_MIN / 10)
				&& (digit - '0') > -(LLONG_MIN % 10)))
			return (1);
	}
	return (0);
}

static int	parse_sign(const char *str, int *i)
{
	int	sign;

	sign = 1;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

int	is_numeric(const char *str)
{
	int			i;
	int			sign;
	long long	result;

	i = 0;
	result = 0;
	if (!str || !str[0])
		return (0);
	sign = parse_sign(str, &i);
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		if (check_overflow(result, str[i], sign))
			return (0);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (1);
}

void	skip_whitespace(const char **s)
{
	while (**s == ' ' || **s == '\t')
		(*s)++;
}
