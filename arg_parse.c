/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 15:35:46 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/18 10:28:10 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int check_arg(char *argv);

static int  fill_args(char **argv, t_args *out)
{
    if(!check_arg(argv[1]) || !check_arg(argv[2])
		|| !check_arg(argv[3]) || !check_arg(argv[4])
		|| !check_arg(argv[5]) || !check_arg(argv[6])
        || !check_arg(argv[7])
	)
        return (write(2, "Invalid Argument\n", 17), -1);
    out->nb_coders = atoi(argv[1]);
    out->time_to_burnout = ft_atol(argv[2]);
    out->time_to_compile = ft_atol(argv[3]);
    out->time_to_debug = ft_atol(argv[4]);
    out->time_to_refactor = ft_atol(argv[5]);
    out->nb_compiles_required = atoi(argv[6]);
    out->dongle_cooldown = ft_atol(argv[7]);
    return (0);
}

int parse_args(int argc, char **argv, t_args *out)
{
    if (argc != 9)
        return (write(2, "\033[91mArgument Missing\033[0m", 25), -1);
    if (fill_args(argv, out) == -1)
        return (-1);
    if (strcmp(argv[8], "fifo") == 0)
        out->scheduler = FIFO;
    else if (strcmp(argv[8], "edf") == 0)
        out->scheduler = EDF;
    else
        return (write(2, "Choose between 'fifo' or 'edf'\n", 31), -1);
    return (0);
}
	
static int check_arg(char *argv)
{
	int i;

	i = 0;
	while (argv[i] != '\0')
	{
		if (argv[i] < '0' || argv[i] > '9')
			return 0;
		i++;
	}
	return 1;
}
	
int validate_args(const t_args *a)
{
    if (a->nb_coders < 1)
        return (write(2, "Can't simulate 0 coders\n", 24), -1);
    if (a->time_to_burnout <= 0)
        return (write(2, "Time to burnout can't be 0\n", 27), -1);
    if (a->time_to_compile <= 0)
        return (write(2, "Time to compile can't be 0\n", 27), -1);
    if (a->time_to_debug <= 0)
        return (write(2, "Time to debug can't be 0\n", 25), -1);
    if (a->time_to_refactor <= 0)
        return (write(2, "Time to refactor can't be 0\n", 28), -1);
    if (a->nb_compiles_required < 1)
        return (write(2, "Need at least 1 compile\n", 24), -1);
    if (a->dongle_cooldown < 0)
        return (write(2, "dongle_cooldown can't be negative\n", 34), -1);
    return (0);
}
