/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 15:35:35 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/19 09:26:44 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_space(char c)
{
	return (c == ' ' || c == '\f'
		|| c == '\n' || c == '\r'
		|| c == '\t' || c == '\v');
}

long	ft_atol(char *str)
{
	int		i;
	int		sign;
	long	nb;

	i = 0;
	while (str[i] && is_space(str[i]))
		i++;
	sign = 1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	nb = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	return (nb * sign);
}

char	*ft_strdup(char *s)
{
	int		i;
	int		size;
	char	*new_s;

	size = strlen(s);
	i = 0;
	new_s = (char *) malloc(sizeof(char) * (size + 1));
	if (!new_s)
		return (NULL);
	else
	{
		while (i < size)
		{
			new_s[i] = s[i];
			i++;
		}
	}
	new_s[i] = '\0';
	return (new_s);
}

// debugggg
int     main(int argc, char **argv)
{
	// parse_args → sim_init → sim_run → sim_destroy → exit
	t_args  args;
	t_sim sim;
	int result;

    if (parse_args(argc, argv, &args) == -1)
        return (1);
    if (sim_init(&sim, &args) != 0)
		return (1);
    result = sim_run(&sim);
	sim_destroy(&sim);
	return (result == 0) ? 0 : 1;
}