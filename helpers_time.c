/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_time.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:08 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/18 11:51:51 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long    now_ms(void)
{
	// Returns current wall-clock time in milliseconds (gettimeofday wrapper)
	struct timeval tv;
	
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
    

long    elapsed_ms(long since_ms)
{
	// Returns now_ms() - since_ms
	return (now_ms() - since_ms);
}

void    sleep_ms(long ms)
{
	// Wraps usleep(); clamps to 0 if ms <= 0
	long convert;
	
	if (ms <= 0)
		return;
	convert = ms * 1000;
	usleep(convert);
}

char	*ft_itoa(int n)
{
	long	nb;
	int		lenght;
	char	*str;
	int		sign;

	nb = n;
	sign = (n < 0);
	if (nb == -2147483648)
		return (ft_strdup("-2147483648"));
	if (sign)
		nb = -nb;
	lenght = strlen(nb) + sign;
	str = (char *)malloc(sizeof(char) * lenght + 1);
	if (!str)
		return (NULL);
	str[lenght] = '\0';
	while (lenght > sign)
	{
		lenght--;
		str[lenght] = (nb % 10) + '0';
		nb = nb / 10;
	}
	if (sign)
		str[0] = '-';
	return (str);
}

static char	*ft_strdup(char *s)
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