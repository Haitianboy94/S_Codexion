/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 15:35:35 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/12 11:17:23 by rulouis          ###   ########.fr       */
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

// debugggg
int     main(int argc, char **argv)
{
	// parse_args → sim_init → sim_run → sim_destroy → exit
	t_args  args;

    if (parse_args(argc, argv, &args) == -1)
        return (1);
    if (validate_args(&args) == -1)
        return (1);
    printf("nb_coders: %d\n", args.nb_coders);
    printf("time_to_burnout: %ld\n", args.time_to_burnout);
    printf("time_to_compile: %ld\n", args.time_to_compile);
    printf("time_to_debug: %ld\n", args.time_to_debug);
    printf("time_to_refactor: %ld\n", args.time_to_refactor);
    printf("nb_compiles_required: %d\n", args.nb_compiles_required);
    printf("dongle_cooldown: %ld\n", args.dongle_cooldown);
    printf("scheduler: %s\n", args.scheduler == FIFO ? "FIFO" : "EDF");
    long res = now_ms();
    printf("time in ms: %ld",res);
    return (0);
}
