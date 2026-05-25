/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_time.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:08 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/18 17:05:41 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long    now_ms(void)
{
	struct timeval tv;
	
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
    

long    elapsed_ms(long since_ms)
{
	return (now_ms() - since_ms);
}

void    sleep_ms(long ms)
{
	long start;

    if (ms <= 0)
        return;
    start = now_ms();
    while (now_ms() - start < ms)
    {
		
	}
}

static	int	lenn(int n)
{
	int	size;

	size = (n <= 0);
	while (n)
	{
		n = n / 10;
		size++;
	}
	return (size);
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
	lenght = lenn(nb) + sign;
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
