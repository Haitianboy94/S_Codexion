/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 15:35:35 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/11 12:47:35 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
    return (0);
}
